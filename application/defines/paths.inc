<?php

if(!defined('DS')){
	define('DS', DIRECTORY_SEPARATOR);
}

if(!defined('INC_PATH')){
	define('INC_PATH', realpath(dirname(__FILE__).DS.'..'.DS.'..'.DS));
}

if(!defined('DIR_CLASSES')){
	define('DIR_CLASSES', INC_PATH.DS.'application'.DS.'classes'.DS);
}

if(!defined('DIR_DEFINES')){
	define('DIR_DEFINES', INC_PATH.DS.'application'.DS.'defines'.DS);
}

if(!defined('DIR_FUNCTIONS')){
	define('DIR_FUNCTIONS', INC_PATH.DS.'application'.DS.'function'.DS);
}

if(!defined('DIR_EXTERNAL')){
	define('DIR_EXTERNAL',  INC_PATH.DS.'external'.DS);
}

if(!defined('DATABASE_LAYER')){
	define('DATABASE_LAYER', INC_PATH.DS.'database'.DS);
}

if(!defined('DIR_SKINS')){
	define('DIR_SKINS', INC_PATH.DS.'templates'.DS);
}

if(!defined('DIR_CONFIG')){
	define('DIR_CONFIG', SITE_PATH.DS.'config'.DS);
}
