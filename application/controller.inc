<?php
ob_start();
$install = true;
date_default_timezone_set('UTC');

function exception_handler($exception) {
	echo 'Uncaught exception: ' , $exception->getMessage(), "\n";
}

$class_block_list = array();

require_once INC_PATH.DIRECTORY_SEPARATOR.'application'.DIRECTORY_SEPARATOR.'defines'.DIRECTORY_SEPARATOR.'paths.inc';

require_once DIR_DEFINES.DS.'settings.inc';

require_once DIR_FUNCTIONS.'autoloader.inc';

controller::singleton()->run();
