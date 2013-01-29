<?php
class content {
	private static $instance;
	
	private $elements;
	private $dom;
	
	private $debug;
	private $debug_data;

	private $warnings;
	private $warnings_data;

	private $head;
	private $head_data;
	
	private $content;
	private $content_data;
	
	private $security;
	private $security_data;
	
	public static function Singleton(){
	   if (!isset(self::$instance)) {
		   $c = __CLASS__;
		   self::$instance = new $c;
	   }

	   return self::$instance;
	}

	protected function __construct()
	{
		$this->elements_attached = false;
		$this->dom = new DOMDocument();
		$this->dom->formatOutput = true;
		
		$this->page = $this->dom->createElement('page');
		$this->page_data = $this->dom->createDocumentFragment();

		$this->content = $this->dom->createElement('body');
		$this->content_data = $this->dom->createDocumentFragment();

		$this->security_data = $this->dom->createDocumentFragment();
		$this->settings_data = $this->dom->createDocumentFragment();
		$this->user_data = $this->dom->createDocumentFragment();
		$this->options_data = $this->dom->createDocumentFragment();
		$this->warnings_data = $this->dom->createDocumentFragment();

		$this->debug = $this->dom->createElement('debug');
		$this->debug_data = $this->dom->createDocumentFragment();
	}

	public function add_custom_head($tag, $attributes, $value = NULL){
		if(!is_array($this->head)){
			$this->head = array();
		}

		$values = array('tag' => $tag, 'attributes' => $attributes, 'value'=>$value);

		//FIXME Dirty hack to prevent values been entered multiple time
		$this->head[sha1(serialize($values))] = $values;
	}
	
	public function build(){
		//Call the template class as it loads the template which might make changes to the settings
		$template = Template::Singleton();
		
		$this->attach_head();
		$this->attach_settings();
		$this->attach_user();
		$this->attach_security();
		$this->attach_warnings(); //This should be the last but one
		$this->attach_debug(); //This should be the last one
		$this->build_xml();
		
		return $this->dom->saveXML();
	}
	
	public function set_attribute($name, $value){
		if(is_array($value)) { $value = reset($value); }
		$this->page->setAttribute($name, @utf8_encode($value));
	}
	
	private function attach_head(){
		if(class_exists('settings')){
			if(isset($this->head))
			{
				$head = array();
				foreach($this->head as $value)
				{
					array_push($head, $value);
				}
	
				Settings::Singleton()->new_setting('header_extra', $head);
			}
		}
	}
	private function attach_security(){
		if(class_exists('Security')){
			$this->security_data->appendXML(Security::Singleton()->security_as_xml());
		}
	}
	
	
	private function attach_debug(){
		if(class_exists('Debug')){
			$this->debug_data->appendXML(@utf8_encode(Debug::Singleton()->render()));
			$this->debug->appendChild($this->debug_data);
		}
	}

	private function attach_settings(){
		if(class_exists('settings')){
			$this->settings_data->appendXML(@utf8_encode(Settings::Singleton()->settings_as_xml()));
		}
	}

	private function attach_warnings(){
		if(class_exists('Warnings')){
			$this->settings_data->appendXML(@utf8_encode(Warnings::Singleton()->get_as_xml()));	
		}
	}

	private function attach_user(){
		if(class_exists('User')){
			$this->user_data->appendXML(@utf8_encode(User::Singleton()->user_details_as_xml()));	
		}
	}
	
	private function attach_options(){
		if(class_exists('options')){
			$this->options_data->appendXML(@utf8_encode(options::Singleton()->options_as_xml()));	
		}
	}

	public function attach_content($xml){
		$xml = str_replace("<?xml version=\"1.0\"?>", '', $xml);
		$this->content_data->appendXML($xml);
		$this->content->appendChild($this->content_data);
		$this->elements_attached = true;
	}

	private function build_xml(){
		if(isset($this->options_data)){
			@$this->page->appendChild($this->content);
		}

		if(isset($this->options_data)){
			@$this->page->appendChild($this->options_data);
		}
		
		if(isset($this->user_data)){
			@$this->page->appendChild($this->user_data);
		}
		
		if(isset($this->settings_data)){
			@$this->page->appendChild($this->settings_data);
		}
		
		if(isset($this->security_data)){
			@$this->page->appendChild($this->security_data);
		}
		
		if(isset($this->head_data)){
			@$this->page->appendChild($this->head_data);
		}
	
		if(isset($this->debug)){
			@$this->page->appendChild($this->debug);
		}

		$this->dom->appendChild($this->page);
	}
}