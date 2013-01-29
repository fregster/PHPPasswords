<?php
class request
{
	protected static $instance;
	protected $require_valid_user = true;

	public static function Singleton($force_type = null){
	   if (!isset(self::$instance)){
	   		if($force_type == null && isset(controller::Singleton()->path_vars[0]))
	   		{
	   			$force_type = controller::Singleton()->path_vars[0];
	   		}
			$c = __CLASS__.'_'.$force_type;
			
	   		if(!class_exists($c))
	   		{
	   			$c = __CLASS__.'_default';
	   		}

		   self::$instance = new $c;
	   }

	   return self::$instance;
	}
	
	protected function __construct(){
		$this->init();
	}
	
	public function __clone(){
	   trigger_error('Clone is not allowed.', E_USER_ERROR);
	}
	
	protected function init(){
		die('Error');
	}
}