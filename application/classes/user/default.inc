<?php
class user_default extends user {
	protected function init(){
		session_start();
		
		if(!isset($_SESSION['IS_LOGGED_IN'])){
			if($this->check_login()){
				if(!$this->do_login()){
					return false;
				}
			}
		}
		
		$this->load_from_cache();
	}
	
	protected function do_login(){
		foreach ($this->login_array as $key => $value){
			if(!post::variable($key, 'isset')){
					die('Not filled in all fields '.$key);
			}
		}
		
		$user_db = database::Singleton()->sql_select_first(NULL, 'tbl_users', 'username', post::variable('username', 'string'));
		
		if($user_db == false or !isset($user_db['id']) or (sha1($user_db['id'].sha1($user_db['salt'].post::variable('password')).MP_SALT) != $user_db['password'])){
			return false;
		}
		
		
		$this->user_details['uid'] = $user_db['id'];
		$this->user_details['username'] = post::variable('username');
		$this->user_details['groups'] = database::Singleton()->sql_select('gid', 'tbl_user_group_membership', 'uid', $this->user_details['uid']);
		$this->user_details['friendly_name'] = $user_db['name'];
		$this->user_details['administrator'] = $user_db['admin'];

		$_SESSION['IS_LOGGED_IN'] = true;
		$this->cache_user();
	}
	
	protected function cache_user(){
		$_SESSION['user_details'] = $this->user_details;
	}
	
	protected function load_from_cache(){
		$this->user_details =& $_SESSION['user_details'];
	}
}