<?php
class controller
{
	private static $instance;
	
	private $template;
	private $render;
	
	public $path_vars = array();
	
	public static function Singleton(){
	   if (!isset(self::$instance)) {
		   $c = __CLASS__;
		   self::$instance = new $c;
	   }

	   return self::$instance;
	}

	public function __clone(){
	   trigger_error('Clone is not allowed.', E_USER_ERROR);
	}

	protected function __construct(){
		$this->path_vars = Common::get_path_vars();
		Settings::singleton()->new_setting('search', true);
		Settings::singleton()->new_setting('char_set', CHAR_SET);
	}
	
	public function run(){
		$config_file = INC_PATH . '/writable/config.inc';
		if(!file_exists($config_file) || !include_once($config_file)){
			die('Run the installation process');
		}
		$this->check_passwords();		
		request::Singleton();		
	}
	
	private function check_passwords(){
		if(masterpassword::Singleton()->get_password() == false || masterpassword::Singleton()->get_password_secondary() == false){
			$this->path_vars[0] = 'initialise';
		}
	}
	
	public function end(){
		
	}
	
	public function path_vars($position){
		if(isset($this->path_vars[$position])){
			return $this->path_vars[$position];
		}
		return false;
	}
}    
