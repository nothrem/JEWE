<?php

/**
 * @brief JEWE plugin that adds title element into HTML head
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Config;
use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;

/**
 * @brief plugin class that adds a title element into the HTML head
 */
class Title implements Plugin, Observer {

	public static function getVersion() : float
	{
		return 1.0;
	}


	public function init(int $kind) : bool
	{
		return true;
	}


	public static function getEvents() : array
	{
		return [
			Events::ON_HEADER,
		];
	}


	public function observe($event, $params) : ?string
	{
		$title = Config::get('title');

		if ($event === Events::ON_HEADER) {
			$title = '<title>' . $title . '</title>' . "\n";
			$title .= '<meta name="generator" content="JEWE, jewe.nothrem.cz">' . "\n";
			$title .= '<meta http-equiv="content-type" content="text/html; charset=utf-8">' . "\n";
			return trim($title);
		} // switch

		return null;
	}

} //class Title

//Register this class as an Observer
Plugins::register(Plugins::OBSERVER, 'Title');
