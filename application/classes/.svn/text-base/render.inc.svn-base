<?php
class render {

	private static $instance;
	private $template_folder;
	private $html;

	public static function Singleton(){
	   if (!isset(self::$instance)) {
		   $c = __CLASS__;
		   self::$instance = new $c;
	   }

	   return self::$instance;
	}
	
	public function send_css(){
		die(Template::Singleton()->send_css());
		exit;
	}
	
	public function render_this($input, $htmldecode = false)
	{
		die($input);
		exit;
	}
	
	public function render_html($xml){
		// Load the site XML pass directly into Render
		if(get::variable('xml','int') or Settings::singleton()->get_setting('send_as_xml') or class_exists('xsltProcessor') === false)
		{
			//Just send the XML
			header('content-type: text/xml');

			$search = '<?xml version="1.0"?>';
			$replace = '<?xml version="1.0" encoding="'. Settings::singleton()->get_setting('char_set').'" ?><?xml-stylesheet type="text/xsl" href="'. Settings::singleton()->get_setting('document_root').'/xslt" ?>';
			if(get::variable('xml','int') == 1) { $replace = '<?xml version="1.0" encoding="'. Settings::singleton()->get_setting('char_set').'" ?>'; }

			$this->render_this(str_replace($search, $replace, $xml));
			exit;
		}
		
		$this->html = $this->transform(Template::Singleton()->load(), $xml);
		
		/* hack to clean html */
//		$this->clean_html();

		/* output the html */
		die($this->html);
	}
	
	private function transform($template, $xml){
		if(strlen($xml) > 0 && strlen($template) > 0)
		{
			$xslt = new xsltProcessor;
			$xslt->registerPHPFunctions();
	
			// Load the documents and process using $xslt
			$xslt->importStyleSheet(@DomDocument::loadXML($template));
			
			/* transform the xml document */
			return $xslt->transformToXML(@DomDocument::loadXML($xml));
		}
		return null;
	}
	
	private function clean_html(){
		$remove = array('xmlns:php="http://www.php.net/xsl"',
						'xmlns:exslt="http://exslt.org/common"',
						'<?xml version="1.0" encoding="'.CHAR_SET.'"?>');

		/*$remove = array('<?xml version="1.0" encoding="UTF-8"?>');*/

//		$start = strpos($this->html, '<!DOCTYPE');
//		$this->html = template::singleton()->dtd().str_replace($remove, '', substr($this->html, (strpos($this->html, '>', $start) +1)));

		$this->tidy();
		$this->compact();
	}
	
	private function compact(){
		$compactor = new Compactor(array('use_buffer' => false,'buffer_echo' => false,'compact_on_shutdown' => false)); 
		$this->html = $compactor->squeeze($this->html);
	}

	private function tidy(){
		if(TIDY_HTML and (function_exists('tidy_parse_string'))){
			/**
			 * Set configuration options for HTML tidy.
			 */
			$config = array('indent' => TRUE,
							'output-xhtml' => TRUE,
							'wrap' => 200,
							'doctype' => 'strict');

			/**
			 * Run HTML tidy on the html code
			 */
			$tidy = tidy_parse_string($this->html, $config, 'UTF8');
			$tidy->cleanRepair();

			$this->html = $tidy;
		}
	}

}