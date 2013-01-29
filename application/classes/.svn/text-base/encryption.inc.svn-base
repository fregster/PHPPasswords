<?php
class encryption{
	
	static public function encrypt($input){
		return encryption::encrypt_aes(encryption::encypt_blowfish($input));
	}
	
	static public function decrypt($input){
		return encryption::decrypt_blowfish(encryption::decrypt_aes($input));
	}
	
	static private function encrypt_aes($input){
		return trim(base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, encryption::key(), $input, MCRYPT_MODE_ECB, mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND)))); 
	}
	
	static private function encypt_blowfish($input){
		return trim(base64_encode(mcrypt_encrypt(MCRYPT_BLOWFISH, encryption::key_secondary(), $input, MCRYPT_MODE_CBC, substr(md5(encryption::key(),true), 0, 8 )))); 
	}
	
	static private function decrypt_aes($input){
		return trim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, encryption::key(), base64_decode($input), MCRYPT_MODE_ECB, mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND))); 
	}
	
	static private function decrypt_blowfish($input){
		return trim(mcrypt_decrypt(MCRYPT_BLOWFISH, encryption::key_secondary(), base64_decode($input), MCRYPT_MODE_CBC, substr(md5(encryption::key(),true), 0, 8 ))); 
	}
	
	static private function key(){
		return md5(masterpassword::Singleton()->get_password().MP_SALT);
	}
	
	static private function key_secondary(){
		return md5(masterpassword::Singleton()->get_password_secondary().MP_SALT);
	}
}