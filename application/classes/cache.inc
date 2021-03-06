<?php
/**
 * 
 * Class to define and send Caching headers for objects
 * @author Paul
 *
 */
abstract class Cache
{
	public static function headers_allow_cache($path = NULL, $public = true, $expires = 31536000, $maxage = 31536000, $force_resend = false)
	{
		if($path != NULL) 
		{ 
			if(!$force_resend) {
			Cache::headers_check_304($path); 
			}
			
			@$mtime = filemtime($path);
			$gmdate_mod = gmdate('D, d M Y H:i:s', $mtime) . ' GMT';
			header('Last-Modified: ' . $gmdate_mod);
			if(Settings::Singleton()->get_setting('cache_use_ETags'))
			{
				Cache::headers_send_etag($path);
			}
		}
		
		
		$expires = gmdate('D, d M Y H:i:s', (time() + $expires)) . ' GMT';
		cache::send_headers($gmdate_mod, $expires, $public);
		header('Cache-Control: max-age='.$maxage, false);
		header('Content-Description: File serverd from a Spensierato site ');

	}
	
	public static function headers_allow_cache_dynamic($page_id, $mod_date, $e_string = null, $public = true, $expires = 0, $maxage = 0, $force_resend = false)
	{
		//Build E-Tag from page id and date
		$ETag = $page_id .'-'.$e_string.'-'.$mod_date;
		if(!$public){
			$ETag .= '-'.user::Singleton()->id();
		}
		
		$gmdate_mod = gmdate('D, d M Y H:i:s', $mod_date) . ' GMT';
		$expires = gmdate('D, d M Y H:i:s', (time() + $expires)) . ' GMT';
		
		if((!$force_resend) or (class_exists('options') && (options::Singleton()->get_option('force_reload'))) )
		{
			if(class_exists('options') && (options::Singleton()->get_option('force_reload'))){
				options::Singleton()->set_option('force_reload', false);
			}
			
			//Headers recieved from browser
			$headers = apache_request_headers();
			
			if(isset($headers['If-None-Match']))
			{
				 if ( ($ETag == str_replace(array('"', "'"), '', $headers['If-None-Match'])) and ($gmdate_mod == $headers['If-Modified-Since']) )
				 {
					Cache::send_304($ETag);
				}
			}
		}

		cache::send_headers($gmdate_mod, $expires, $public, $ETag);
		header('Cache-Control: max-age='.$maxage, false);
		header('Content-Description: File serverd from a Spensierato site ');
		
	}
	
	public static function send_headers($gmdate_mod, $expires, $public = true, $ETag = null)
	{
		if($public == true)
		{
			$cache_control = 'public, must-revalidate';
		}
		else
		{
			$cache_control = 'private, no-cache, must-revalidate';
		}
		
		if(Settings::Singleton()->get_setting('cache_use_ETags') && $ETag != null)
		{
			header('ETag: "'.$ETag.'"');
		}
		header('Last-Modified: ' . $gmdate_mod);
		header('Expires: ' . $expires);
		header('Cache-Control: post-check=0, pre-check=0');
		header('Cache-Control: '.$cache_control, false);
		header('Pragma: '.$cache_control);
		
	}
	
	private static function generate_etag($path)
	{
		// Inode
		$ETag = @dechex(fileinode($path));
		
		//Quick check that the Inode worked
		if($ETag != 0)
		{
			// Size
			$ETag.= "-".dechex(filesize($path));
			// Modification time in useconds & (2^33-1)
			$ETag.= "-".dechex(((filemtime($path).str_repeat("0",6)+0) & (8589934591)));
			
			return $ETag;
		}
		return false;
	}
	
	private static function headers_send_etag($path)
	{
		$ETag = Cache::generate_etag($path);
		if($ETag != false && $ETag != '')
		{
			header('ETag: "'.$ETag.'"'); 
		}
	}
	
	public static function headers_no_cache()
	{
		$expires = gmdate('D, d M Y H:i:s', (time() - 36000)) . ' GMT';
		
		header('Cache-Control: private, no-cache, must-revalidate'); // HTTP/1.1
		header('Expires: ' . $expires); // Date in the past
		header('pragma: private'); 
	}
	
	public static function headers_check_304($path)
	{
		//File modification time
		$gmdate_mod = @gmdate('D, d M Y H:i:s', filemtime($path)) . ' GMT';
		
		//File ETag
		$ETag = Cache::generate_etag($path);
		
		//Headers recieved from browser
		$headers = apache_request_headers();
		
		if(isset($headers['If-None-Match']))
		{
			 if (strpos($headers['If-None-Match'], $ETag) != false)
			 {
				Cache::send_304($ETag);
			}
		}
		
		if(isset($headers['If-Modified-Since']))
		{
			if ($gmdate_mod == $headers['If-Modified-Since']) 
			{
				Cache::send_304();
			}
		}
	}
	
	public static function headers_check_custom_etag($ETag, $gmdate_mod = NULL)
	{		
		//Headers recieved from browser
		$headers = apache_request_headers();
		
		if(isset($headers['If-None-Match']))
		{
			 if (strpos($headers['If-None-Match'], $ETag) != false)
			 {
				Cache::send_304($ETag);
			}
		}
		
		if (isset($headers['If-Modified-Since']) && $gmdate_mod == $headers['If-Modified-Since']) 
		{
			Cache::send_304();
		}
	}
	
	Public static function send_304($ETag = NULL)
	{
		// They already have an up to date copy so tell them
		header('HTTP/1.1 304 Not Modified');
		header("Pragma: ");
		header('Expires: ');
		header('Content-Type: ');
		header('Cache-Control: ');
		if($ETag != NULL)
		{
			header('ETag: "'.$ETag.'"');
		}
		die(); 
	}

}