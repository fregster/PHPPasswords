<?php
class request_default extends request{
	
	protected $require_valid_user = false;
	
	protected function init(){
		user::Singleton();
		$render = new render;
		$template = new template;
		
		if(get::variable('search-quick', 'is_set')){
			content::Singleton()->attach_content(common::strip_external_xml_decleration(search::Singleton()->search(get::variable('search-quick'))));
		}
		
				
		$word = 'blahsy blah ~#&*""\'$%^&*()!¬{}[];:@><,.|\`+-';
		//echo($word.'<br/>');
		
		$encrpytion = encryption::encrypt($word);
//		echo($encrpytion.'<br/>');
		
		if(get::variable('view', 'is_set')){
			$record = Database::Singleton()->sql_select_first('*', 'tbl_password', array('id'), array(1));
			$record['decrypted'] = utf8_encode('<![CDATA['.encryption::decrypt($record['data']).'//]]>');
//			$record['decrypted'] = encryption::decrypt($record['data']);
			content::Singleton()->attach_content(xml::array_to_xml_simple($record, 'password'));
			unset($record);
		}
		
		Render::Singleton()->render_html(Content::Singleton()->build(), true);
	}
}