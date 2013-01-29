<?php
class request_initialise extends request{
	public function __construct(){
		if(masterpassword::Singleton()->get_password() == false){
			echo('<h3>Master Key not set, setting key</h3>');
			if(!masterpassword::Singleton()->set_password('vmko238ryosfh1nmb')){
				die('Failed to set password');
			}
		}
		
		if(masterpassword::Singleton()->get_password_secondary() == false){
			echo('<h3>Secondary Key not set, setting key</h3>');
			if(!masterpassword::Singleton()->set_password_secondary('vmko238ryosfh1nmb')){
				die('Failed to set password');
			}
		}
	}
}