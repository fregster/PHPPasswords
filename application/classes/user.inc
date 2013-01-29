<?php
class user {
	private static $instance;
	
	protected $login_array = array('username' => 'text', 'password' => 'password');
	
	protected $user_details = array('uid' => 0, 'groups' => array(), 'username' => '', 'friendly_name' => '');
	
	public static function Singleton($force_type = null){
	   if (!isset(self::$instance)){
	   		if($force_type == null){
	   			$force_type = LOGIN_BACKEND;
	   		}
			$c = __CLASS__.'_'.$force_type;
			
	   		if(!class_exists($c)){
	   			$c = __CLASS__.'_default';
	   		}

		   self::$instance = new $c;
	   }

	   return self::$instance;
	}
	
	protected function __construct(){
		$this->init();
	}
	
	protected function init(){
		
	}
	
	public function id(){
		return $this->user_details['uid'];
	}
	
	public function user_details_as_xml(){
		return xml::array_to_xml_simple($this->user_details, 'user', 'group');
	}
	
	protected function check_login(){
		if($_SERVER['REQUEST_METHOD'] == 'POST'){
			foreach ($this->login_array as $key => $value){
				if(!post::variable($key, 'isset')){
					die('Not filled in all fields '.$key);
				}
			}
			return true;
		}
		return false;
	}
	
	protected function do_login(){
		return false;
	}
}