<?php
class Settings
{
	private $settings = array();
	private $db_loaded = false;

	private static $instance;

	public static function singleton()
	{
	   if (!isset(self::$instance))
	   {
		   $c = __CLASS__;
		   self::$instance = new $c;
	   }

	   return self::$instance;
	}

	protected function __construct()
	{
		//Force some default settings
		$document_root = Common::document_root();
		
		$this->new_setting('document_root', $document_root);
		
		$this->new_setting('static_host_ajax', $document_root.'/ajax');

		$this->new_setting('static_host_skin', $document_root);

		$this->new_setting('static_host_js', $document_root);
	}

	public function __clone()
	{
	   trigger_error('Clone is not allowed.', E_USER_ERROR);
	}

	//Update the settings with those from the DB
	public function get_database_settings()
	{
		if(!$this->db_loaded)
		{
			$this->db_loaded = true;
			//TODO write if load from db do this else skip
			$settings = Database::Singleton()->sql_select(array('setting', 'value'), 'tbl_settings');

			if(	($this->get_setting('protect_db_settings') != true) and ($this->get_setting('protect_db_settings') != false) )
			{
				die('must be true or false');
			}

			if(is_array($settings))
			{
				foreach($settings as $setting)
				{
					$this->overwrite_setting($setting['setting'], $setting['value'], $this->get_setting('protect_db_settings'));
				}
			}
		}
	}

	public function new_setting($name, $value, $protected = false, $hidden = false, $editable = false, $edit_group = null)
	{
			if(!isset($this->settings[$name]))
			{
				$this->settings[$name] = array();
			}
			$setting_details = array('protected' => $protected, 'hidden' => $hidden, 'value' => $value, 'editable' => $editable, 'edit_group' => $edit_group);
			array_push($this->settings[$name], $setting_details);
	}

	public function get_setting($name)
	{
		if(!$this->db_loaded)
		{
			$this->get_database_settings();
		}
		if(isset($this->settings[$name][0]['value']))
		{
			return $this->settings[$name][0]['value'];
		}

		return false;
	}

	public function settings_as_xml()
	{
		return xml::array_to_xml_simple($this->get_all_settings(), 'settings', 'setting');
	}
	
	public function get_all_settings($editable = false)
	{
		$i = 0;
		$settings = array();
		
		/**
		 * Split for performance, increased code size but reduces complex if's majority of the time
		 */
		if(!$editable)
		{
			foreach($this->settings as $setting)
			{
				$key = array_keys($this->settings);
				if($setting[0]['hidden'] != true)
				{
					$settings[$key[$i]] = $setting[0]['value'];	
				}
				$i++;
			}
			return $settings;
		}
		else
		{
			foreach($this->settings as $setting)
			{
				$key = array_keys($this->settings);
				if($setting[0]['editable'] == true)
				{
					if(!isset($setting[0]['edit_group'])){ $setting[0]['edit_group'] = 99999; }
					if(!isset($settings[$setting[0]['edit_group']]))
					{
						$settings[$setting[0]['edit_group']] = array();
					}		
					
					$settings[$setting[0]['edit_group']][$key[$i]] = array('name' => $key[$i], 'value' => $setting[0]['value'], 'options' => $setting[0]['editable']);
				}
				$i++;
			}
			ksort($settings);
			return $settings;
		}
	}

	public function overwrite_setting($name, $value, $protected = false, $editable = false)
	{
		//Check if the setting exists. if not, create a new one.
		if (!isset($this->settings[$name])) {
			$this->new_setting($name, $value, $protected);
			return TRUE;
		}
		//Check the old setting is not protected
		else if($this->settings[$name][0]['protected'] != true)
		{
			//Remove old settings
			$hidden = $this->settings[$name][0]['hidden'];
			unset($this->settings[$name]);

			//Push the new setting
			$this->new_setting($name, $value, $protected, $hidden, $editable);

			return true;
		}

		return false;
	}
}