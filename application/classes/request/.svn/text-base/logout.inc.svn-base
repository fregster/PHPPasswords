<?php
class request_logout extends request {
	public function __construct(){
		session_start();
		session_destroy();
		header('Redirect: /');
		die();
	}
}