<?php
/**
* PHP Persistant Caching Interface
*
* Requirements: PHP5
* Sugested: php5-memcache and memcached
*
* Copyright (c) 2009 Paul Fryer (www.fryer.org.uk)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the The GNU Lesser General Public License as published by
* the Free Software Foundation; version 3 or any latter version of the license.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* The GNU Lesser General Public License (LGPLv3) for more details.
*
*
* @package PHPPersistantCache
* @author Paul Fryer <paul@fryer.org.uk>
* @license http://www.opensource.org/licenses/lgpl-3.0.html LGPL
* @link http://www.fryer.org.uk/
*
* Version 2.1 Released Dec 2009
*/

if(!defined('PREFIX'))
{
	define('PREFIX', 'cache_'.$_SERVER['SERVER_NAME'].'_'); //Prefix the key names to help prevent collisions
}

/**
 * Persistant Cache is a monolithic file for fast caching of data
 * Supports memcache with fall back to file and then session caching
 *
 */
class persistantcache
{
	private $encrypt_data = false;
	private $encryption_key = 'mnbvcfde4567890poiuytrazsxv';
	private $encryption_type = array('MCRYPT_RIJNDAEL_256', 'MCRYPT_TWOFISH256', 'MCRYPT_3DES', 'MCRYPT_CRYPT'); //In order of preference
	protected $max_cache_time = 300;
	protected $max_object_size = 20480; //The string length of the serialized array
	protected static $retry_time = 30; //Time in seconds before reworking out the best cache type

	protected $allow_memcache = true;
	protected $allow_file_cache = true;
	protected $allow_session_cache = true;
	protected static $cache_preference_order = array('memcache', 'file', 'session');

	/**
	 * DO NOT CHANGE THESE
	 */
	protected $enabled = false; //Don't change this it is set in the constructors if connected
	protected $easy_cache_type_name = 'none';
	protected static $time; //Local cache of the time
	public static $cache_type_inuse;
	private static $instance;

	public function __clone()
	{
	   trigger_error('Clone is not allowed.', E_USER_ERROR);
	}

	protected function __construct()
	{
		$this->init();
		return false; //Unable to use a persistant cache
	}

	//Standard init function
	protected function init()
	{
		//Internaly cache time
		$this->time = time();
	}

	public static function Singleton()
	{
		if (!isset(self::$instance)) //No instance found so call rebuild to build a new one
		{
			persistantcache::rebuild_singleton();
		}

	   return self::$instance;
	}

	/**
	 * Rebuild Singleton
	 * This actually replaces the current instance with the best available cache type
	 * @return nothing
	 */
	protected static function rebuild_singleton()
	{
		$c =persistantcache::get_type();
		self::$instance = new $c;
	}

	/**
	 * Work out the best avaiable cache type
	 * @return class name
	 */
	private static function get_type()
	{
		$c = __CLASS__;
//		unset($_SESSION['persistant_cache_type']);
		if(isset($_SESSION['persistant_cache_type']) && class_exists($_SESSION['persistant_cache_type']['type'])) { $c = $_SESSION['persistant_cache_type']['type']; } //If a cache type is set ensure it is still available

    	if(!isset($_SESSION['persistant_cache_type']) || $_SESSION['persistant_cache_type']['time'] > (persistantcache::$time - persistantcache::$retry_time)) //Check if we have recently checked for a cache type this is per session checking, but still reduces load
		{
	    	foreach(persistantcache::$cache_preference_order as $type) //Run through the available types of cache and test
			{
				$class = 'persistantcache_'.$type;
				if(class_exists($class))
				{
					$try = new $class;
					if($try->enabled())
					{
						$c = $class;
						$_SESSION['persistant_cache_type'] = array('type'=>$class, 'time'=>persistantcache::$time);
						break; //The best cache has been selected escape
					}
				}
			}
		}
		return $c;
	}

	public function version()
	{
		return 0;
	}

	/**
	 * Return true if an advanced caching system is in use else false for session or no cache
	 */
	public function enabled()
	{
		return $this->enabled;
	}

	public function type()
	{
		return $this->easy_cache_type_name;
	}

	/**
	 * Build the key name
	 * As memcache can support a key with a max length of 240 bytes build a sha1 hash (256 bits)
	 * Should be safe as there are no currently known collisions in sha1
	 */
	protected function build_key($name)
	{
		return sha1(PREFIX.$name);
	}

	/**
	 * If encrypt is enabled encrypt the data else return the data un touched
	 * @param $data
	 * @return encrypted data
	 */
	protected function encrypt($data)
	{
		//TODO Write encrypt method
		if($this->encrypt_data)
		{
			$data = $data;
		}
		return $data;
	}

	/**
	 * If encrypt is enabled decrypt the data else return the data un touched
	 * @param $data
	 * @return decrypted data
	 */
	protected function decrypt($data)
	{
		//TODO Test data is encrypted and write decrypt method
		if($this->encrypt_data)
		{
			$data = $data;
		}
		return $data;
	}

	/**
	 * Build the data into a consistant store
	 * @param $name
	 * @param $value
	 * @param $time
	 * @return array();
	 */
	protected function build_save($name, $value, $time)
	{
		//Build the expire time
		$expire_time = $this->time + $this->max_cache_time;
		if($time != 0)
		{
			$expire_time = $expire_time + $time;
		}
		else
		{
			$expire_time = $expire_time + 999999999999999999999999;
		}

		//Build the value
		$data = serialize($value);
		$value = array('value' => $this->encrypt($data), 'time' => $expire_time, 'validate' => crc32($data), 'encrypted' => $this->encrypt_data);

		return array('name'=>$this->build_key($name), 'data'=>$value);
	}

	protected function validate_data($data, $check)
	{
		if(crc32($data) == $check)
		{
			return true;
		}
		return false;
	}

	/**
	 * Public method for saveing data
	 * name
	 * value is what you want to cache
	 * time max storage time
	 */
	public function save($name, $value, $time = 0)
	{
		return false;
	}

	/**
	 * Get an object from the cache by key name
	 */
	public function get($name)
	{
		return false;
	}

	/**
	 * Delete an object from the cache
	 */
	public function drop($name)
	{
		return false;
	}

	/**
	 * Delete every thing from the cache, useful when the database is known to have been updated.
	 */
	public function purge()
	{
		return false;
	}
}

/**
 * persistantcache_memcache
 * 
 * This is the implementation of the persistant cache using Memcached
 * It supports multiple memcached servers and fail over with delayed re-testing
 * 
 * @author Paul Fryer
 *
 */
class persistantcache_memcache extends persistantcache
{
	private $store = false; //Memcache object
	private $servers = array(array('host' => 'localhost', 'port'=>11211)); // enter an array for each of your memcache servers
	private $maxsession_cache_time = 0; //0 sets to never expire

	protected function __construct()
	{
		$this->init();

		if(isset($_SESSION['disable_memcached']) && ($_SESSION['disable_memcached'] + persistantcache::$retry_time) > $this->time )
		{
			return false;
		}

		//Are we using memcached and if so ensure if it recently failed we don't try to reconnect to quickly
		if(extension_loaded('memcache') && $this->allow_memcache)
		{
			$this->store = new Memcache;

			//Run though the array of servers adding each one to the connection pool
			foreach($this->servers as $server)
			{
				$this->store->addServer($server['host'], $server['port'], TRUE, 1, 1, 1, TRUE, 'memcached_server_fail');
			}

			//Mark that we are using an advanced caching mech, purge any session cache
			$this->enabled = true;
			$this->easy_cache_type_name = 'AC';
			if(isset($_SESSION['cache'])) { unset($_SESSION['cache']); } //Clear any session cache
			return true;
		}
		return false;
	}

	public function save($name, $value, $time = 0)
	{
		if(!$this->enabled){ return false; }
		$data = $this->build_save($name, $value, $time);
		if(strlen($data['data']['value']) < $this->max_object_size && $this->store != false)
		{
			$result = $this->store->replace($data['name'], $data['data'], MEMCACHE_COMPRESSED, $data['data']['time']);
			if( $result == false )
			{
			    $result = $this->store->set($data['name'], $data['data'], MEMCACHE_COMPRESSED, $data['data']['time']);
			}
			return $result;
		}
		return false;
	}

	public function get($name)
	{
		if($this->store == false)
		{
			return false;
		}
		$name = $this->build_key($name);
		$result = $this->store->get($name);
		if($result['time'] > $this->time && $this->validate_data($result['value'], $result['validate'])  && $this->store != false)
		{
			return unserialize($this->decrypt($result['value']));
		}
		return false;
	}

	public function drop($name)
	{
		if($this->store != false)
		{
			$name = $this->build_key($name);
			return $this->store->delete($name);
		}
		return false;
	}

	public function purge()
	{
		if($this->store != false)
		{
			return $this->store->flush();
		}
		return false;
	}

	public function version()
	{
		if($this->store != false)
		{
			$this->store->getVersion();
		}
		return false;
	}

	/**
	 * Register that a server has gone offline
	 * Needs work
	 */
	public function server_fail($server = null, $port = null)
	{
		$this->disable_memcached();
	}

	/**
	 * All servers off line, disable and make offline timestamp
	 */
	protected function disable_memcached()
	{
		$this->enabled = false;
		$_SESSION['disable_memcached'] = $this->time;
		$this->rebuild_singleton();
	}
}

/**
 * File system based implementation
 * Currently only this was designed for and tested against Linix
 * Please let me know if it works or you patch it to work against Windows
 * 
 * @author Paul Fryer
 *
 */

class persistantcache_file extends persistantcache
{
	/**
	 * File caching
	 *
	 * You will need to employ some method of cleaning and quotering the max size of any filesystem cache
	 * Please ensure this folder is kept purley for this cache, else the purge and drop might delete your files
	 */

	private $file_cache_path = '/tmp/cms_cache/'; //Remeber trailing /
	private $max_file_size = 20480; //The max string length of the serialized array
	private $file_system_maintance = true;
	private $max_time_before_maintance = 3600; //Seconds before maintance is ran

	protected function __construct()
	{
		$this->init();

		//If we want to use file caching test we can write first.
		if($this->allow_file_cache)
		{
			if(!is_writable($this->file_cache_path))
			{
				$this->allow_file_cache = false;
				return false;
			}
			$this->enabled = true;
			$this->easy_cache_type_name = 'FC';
			if(isset($_SESSION['cache'])) { unset($_SESSION['cache']); } //Clear any session cache
			return true;
		}
		return false;
	}

	public function save($name, $value, $time = 0)
	{
		if(!$this->enabled){ return false; }
		$data = build_save($name, $value, $time);

		if($file = fopen($this->build_file_path($data['name']), 'wb'))
		{
			$result = fwrite($file, $data['data']);
			fclose($file);
			return $result;
		}
		return false;
	}

	public function get($name)
	{
		if(!$this->enabled){ return false; }
		$name = $this->build_key($name);

		if(file_exists($name))
		{
			$result['time'] = filemtime($file);
			$result['value'] = file_get_contents($file);
		}

		if($result['time'] > $this->time && $this->validate_data($result['value'], $result['validate']))
		{
			return unserialize($this->decrypt($result['value']));
		}

		return false;
	}

	public function drop($name)
	{
		$name = $this->build_key($name);
		//TODO Write drop file method
	}

	public function purge()
	{
		//TODO Write purge file method
	}

	private function build_file_path($name)
	{
		return $this->file_cache_path.PREFIX.'_'.$name.'.cache';
	}
}

class persistantcache_session extends persistantcache
{
	/**
	 * Session caching
	 * CAUTION Only use this on site where your going to have a small number of users
	 */
	private $max_objects_in_session_cache = 50; //Prevent the sessions getting to big note there is no way of knowing how many users there are at once.

	protected function __construct()
	{
		$this->init();

		if($this->allow_session_cache)
		{
			$_SESSION['cache'] = array();
			$_SESSION['cache_count'] = 0;
			$this->enabled = true;
			$this->easy_cache_type_name = 'SC';
			return true;
		}
		return false;
	}

	public function save($name, $value, $time = 0)
	{
		if(!$this->enabled){ return false; }
		$data = $this->build_save($name, $value, $time);

		if(strlen($data['data']['value']) < $this->max_object_size)
		{
				//Because the session cache can't self clean, ensure the cache is not too big, if it is drop the oldest one.
				if($_SESSION['cache_count'] > $this->max_objects_in_session_cache)
				{
					$_SESSION['cache'] = array_reverse($_SESSION['cache']);
					array_pop($_SESSION['cache']);
					$_SESSION['cache'] = array_reverse($_SESSION['cache']);
				}
				$_SESSION['cache'][$data['name']] = $data['data'];
				++$_SESSION['cache_count'];
				return true;
		}
		return false;
	}

	public function get($name)
	{
		if(!$this->enabled){ return false; }
		$name = $this->build_key($name);

		$result = isset($_SESSION['cache'][$name])?$_SESSION['cache'][$name]:NULL;

		if($result['time'] > $this->time && $this->validate_data($result['value'], $result['validate']))
		{
			return unserialize($this->decrypt($result['value']));
		}

		//The data in the session cache is outdated so delete it
		unset($_SESSION['cache'][$name]);
		return false;
	}

	public function drop($name)
	{
		if(!$this->enabled){ return false; }
		$name = $this->build_key($name);
		unset($_SESSION['cache'][$name]);
		--$_SESSION['cache_count'];
	}

	public function purge()
	{
		$_SESSION['cache'] = array();
		$_SESSION['cache_count'] = 0;
	}
}

/**
 * Memcached call back function on server fail
 * This is a function as the call back works better
 *
 * @param unknown_type $server
 * @param unknown_type $port
 */
function memcached_server_fail($server = NULL, $port = NULL)
{
	persistantcache::Singleton()->server_fail($server = NULL, $port = NULL);
}