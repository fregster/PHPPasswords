<?php

/**
 * 
 * This is the container class for the master encryption password
 * @author Paul
 *
 */
class masterpassword {
	private static $instance;
	
	private $password;

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
		
	}
	
	/**
	 * 
	 * Returns the master encryption password
	 */
	
	public function get_password(){
		return persistantcache::Singleton()->get('PHPPassword_master_password');
	}
	
	/**
	 * 
	 * Set's the master password
	 */
	public function set_password($password){
		return persistantcache::Singleton()->save('PHPPassword_master_password', hash('sha512', $password.MP_SALT));
	}
	
	public function get_password_secondary(){
		return persistantcache::Singleton()->get('PHPPassword_secondary_password');
	}
	
	public function set_password_secondary($password){
		$hash = hash('sha512', $password.MP_SALT);
		$comparison = new StringMatch;
		$result = round(($comparison->fstrcmp ($hash, strlen($hash), $this->get_password(), strlen($this->get_password()), 0.4)),2);
		
		if($result > 75){
			persistantcache::Singleton()->drop('PHPPassword_secondary_password');
			persistantcache::Singleton()->drop('PHPPassword_master_password');
			die('Passwords are to similar strings match around: '.$result.'%');
		}
		return persistantcache::Singleton()->save('PHPPassword_secondary_password', $hash);
	}
	
}