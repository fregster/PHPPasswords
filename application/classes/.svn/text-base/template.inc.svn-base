<?php
class Template
{
	private static $instance;

	private $template_path;
	private $generic_path;
	private $working_template_path;
	private $child_template;
	private $inherited_template_paths = array();
	private $preload = array();
	private $lastmodified = 0;
	private $xslt;
	private $list_of_files = array();
	private $filenames = array();

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

	public function __construct()
	{
		$this->default_path = DIR_SKINS.'skins'.DS.'default';
		$this->generic_path = DIR_SKINS.'generic';
		
		if(USE_EXTERNAL_SKINS === true && !defined('SKIN') && SKIN){
			$this->template_path = file_root().DS.'skins'.DS.SKIN;
		}
		else if (SKIN){
			$this->template_path = DIR_SKINS.'skins'.DS.SKIN;
		}
		else{
			$this->template_path = $this->default_path;
		}
		
		

		$templateinc = $this->template_path.DS.'template.inc';

		if(file_exists($templateinc)){
			include($templateinc);
			$this->load_template_options();
		}
		$this->working_template_path = $this->template_path;
		$this->inherited_template_paths[] = $this->template_path;
		$this->inherited_template_paths[] = $this->generic_path;
		$this->build_template();		
	}
	
	public function path(){
		return $this->template_path;
	}
	
	public function inhearited_paths(){
		return $this->inherited_template_paths;
	}

	public function get_file_content($file){
		$path = $this->get_file_path($file);
		if($path){
			return Common::get_file_contents_as_string($path);
		}
		return '';
	}
	
	public function get_file_path($file){
		foreach($this->inherited_template_paths as $pos_path){
			$path = $pos_path .DS.$file;
			if(file_exists($path)){
				return $path;
			}
		}
		return false;
	}

	public function load(){
		if(!isset($this->xslt)){
			$this->xslt = $this->build_xslt_template($this->load_xslt_template());
		}
//		die($this->xslt);
		return $this->xslt;
	}

	public function lastmodified(){
		return $this->lastmodified;
	}

	public function dtd(){
		$dtd = array(
		'xhtml' => '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">',
		'transitional' => '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">',
		'transitional-rec' => '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" "http://www.w3.org/TR/REC-html40/loose.dtd">'
		,

		);

		if(isset($dtd[Settings::singleton()->get_setting('template_dtd')])){
			return $dtd[Settings::singleton()->get_setting('template_dtd')];
		}

		return $dtd['xhtml'];
	}

	public function css_path(){
		$path_vars = controller::Singleton()->path_vars(1);
		
		switch ($path_vars){
			case 'mobile':
				$path = $this->default_path.DS.'mobile.css';
			break;

			case 'submodal':
				$path = $this->default_path.DS.'subModal.css';
			break;

			default:
				if(defined('SKIN_ALTERNATIVE_CSS')){
					$css_file = SKIN_ALTERNATIVE_CSS;
				}
				else{
					$css_file = 'css.css';
				}
				foreach($this->inherited_template_paths as $pos_css_path){
					$path = $pos_css_path.DS.'css'.DS.$css_file;
					if(file_exists($path)){
						return $path;
					}
				}
				error::do404();
		}
		return $path;
	}

	public function send_css(){
		if(is_file($this->css_path())){			
			$css_m_time = 0;
			$css_files = Settings::Singleton()->get_setting('template_inherits_css');;
			if(!isset($css_files[0])){
				$css_files = array();
			}
			$css_files[] = $this->css_path();
			$css_paths = array();
			$estring = '';
			foreach($css_files as $file){
				$path = $this->get_file_path('css'.DS.basename($file));
				$css_paths[] = $path;
				$estring .= $path;
				$css_m_time = max($css_m_time, filemtime($path));
			}
			$estring = md5($estring);
	
			$css = persistantcache::Singleton()->get('template_css');
			
			if(!isset($css['mtime']) || $css['mtime'] != $css_m_time || $css['paths'] != $css_paths)
			{
				$css = '@charset "UTF-8";';
				foreach($css_paths as $path)
				{
					$css .= str_ireplace(array('@CHARSET "UTF-8";', '@CHARSET "ISO-8859-1";'), '', file_get_contents($path));
				}
				$css = array('css' => $css, 'mtime' => $css_m_time, 'paths' => $css_paths);
				
				persistantcache::Singleton()->save('template_css', $css);
			}
			
			// send the requisite header information and character set

			header ('Content-type: text/css; charset: '.Settings::Singleton()->get_setting('char_set'));
			Cache::headers_allow_cache_dynamic('css', $css_m_time, $estring, true, 31536000, 31536000);

			if(Settings::singleton()->get_setting('css_minify'))
			{
		    	return(Common::remove_extra_lines(Common::remove_css_comments($css['css'])));
			}
			else
			{
				return($css['css']);
			}
		}
		error::do404();
	}

	private function build_xslt_template($xslt){
		$xslt = '<?xml version="1.0" encoding="'.CHAR_SET.'"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >
<xsl:output method="html" version="1.0" encoding="'.CHAR_SET.'" indent="yes"/>
	'.$xslt.'
</xsl:stylesheet>';

		if(defined('XSLT') && XSLT === true){
			$footer = '<div id="page_section_footer">Page rendered client side</div>';
		}
		else if(defined('SKIN_FOOTER_TEXT')){
			$footer = SKIN_FOOTER_TEXT;
		}
		else{
			$footer = controller::Singleton()->end();
		}

		return str_replace('***FOOTER_TEXT***', $footer, $xslt);

	}

	private function load_xslt_template(){		
		$xslt = persistantcache::Singleton()->get('template_xslt');

		if(!isset($xslt['mtime']) || $xslt['mtime'] != $this->lastmodified)
		{
			$xslt = ''; //drop the array
			foreach($this->filenames as $filepath)
			{
				$xslt .= str_ireplace('<?xml version="1.0" encoding="UTF-8"?>', '', file_get_contents($filepath));
			}
		}
		else
		{
			$xslt = $xslt['xslt']; //Overwrite the array
		}

		persistantcache::Singleton()->save('template_xslt', array('xslt' => $xslt, 'mtime' => $this->lastmodified));

		return $xslt;
	}
	
	private function build_template(){
		$this->extend_template_with(glob($this->template_path.DS.'xslt'.DS.'*.xml'));
		$this->extend_template();
		$this->include_generic_template();
	}
	
	private function template_mtime(){
		
	}
	
	private function extend_template(){
		$old_template = $this->working_template_path;
		if(file_exists($this->working_template_path.DS.'template.inc'))
		{
			require_once($this->working_template_path.DS.'template.inc');
		
			if(SKIN_EXTENDS != false)
			{
				if(file_exists(file_root().'skins'.DS.SKIN_EXTENDS))
				{
					$this->working_template_path = file_exists(file_root().'skins'.DS.SKIN_EXTENDS);
				}
				else
				{
					$this->working_template_path = DIR_SKINS.'skins'.DS.SKIN_EXTENDS;
				}
				Settings::Singleton()->overwrite_setting('template_extends', false, false, true);
				
				if($this->working_template_path != $old_template)
				{
					if(file_exists($this->working_template_path.DS.'template.inc'))
					{
						require_once($this->working_template_path.DS.'template.inc');
					}
					
					$this->extend_template_with(glob($this->working_template_path.DS.'templates'.DS.'*.xml'));
					
					$this->inherited_template_paths[] = $this->working_template_path;
					$this->extend_template();
				}
			}
		}
	}
	
	private function extend_template_with($files){
			foreach ($files as $filepath)
			{
				$filename = basename($filepath);
				if(!isset($this->filenames[$filename]))
				{
					$this->filenames[$filename] = $filepath;
					$this->lastmodified = max($this->lastmodified, filemtime($filepath));
				}
			}
	}
	
	private function include_generic_template(){
		if(!defined('EXCLUDE_GENERIC_TEMPLATES')){
			$this->inherited_template_paths[] = $this->generic_path.DS.'xslt';
			$this->extend_template_with(glob($this->generic_path.DS.'xslt'.DS.'*.xml'));
		}
	}

	private function load_template_options(){
		//Does this template have a header graphic option?
		if(Settings::Singleton()->get_setting('template_header_graphic') != NULL)
		{
			Settings::Singleton()->overwrite_setting('template_header_graphic', Common::get_random_from_array(Settings::Singleton()->get_setting('template_header_graphic')));
		}

		if(isset($this->preload[0]))
		{
			Settings::Singleton()->new_setting('pre_load_images', $this->preload);
		}
	}
}