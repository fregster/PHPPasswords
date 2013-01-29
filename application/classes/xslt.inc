<?php
class xslt {
	public static function transform_xml_with_xslt($xml, $template)
	{
		if(strlen($xml) > 0 && strlen($template) > 0)
		{
			$xslt = new xsltProcessor;
			$xslt->registerPHPFunctions();
	
			// Load the documents and process using $xslt
			$xslt->importStyleSheet(DomDocument::loadXML($template));
	
			/* transform the xml document */
			return $xslt->transformToXML(@DomDocument::loadXML($xml));
		}
		return null;
	}
}