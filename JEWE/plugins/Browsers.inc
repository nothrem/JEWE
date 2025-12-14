<?php

/**
 * @brief JEWE plugin that writes supported browser to the end of the page
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;

/**
 * @brief plugin class that writes supported browser to the end of page
 */
class Browsers implements Plugin, Observer {
	public static function getVersion() : float
	{
		return 1.0;
	}


	public function init(int $kind) : bool
	{
		//menu requires plugin Chapters
		return true;
	}


	public static function getEvents() : array
	{
		return [
			Events::ON_BODY_END,
		];
	}


	public function observe(int $event, ?array $params) : ?string
	{
		$edge = 'https://www.microsoft.com/cs-cz/edge/?form=MA13FJ';
		$ff = 'http://firefox.com';
		$opera = 'http://opera.com';
		$sa3 = 'http://www.apple.com/safari/';
		$chrome = 'http://www.google.com/chrome/';

		if ($event === Events::ON_BODY_END) {
			$text = '<div id="supported_browsers">';
			$text .= 'JEWE supports all HTML5 browsers. Get yours: ';
			$text .= '<a href="' . $edge . '">Microsoft Edge</a>';
			$text .= '<a href="' . $ff . '">Firefox 4+</a>';
			$text .= '<a href="' . $opera . '">Opera 11+</a>';
			$text .= '<a href="' . $sa3 . '">Safari 5+</a>';
			$text .= '<a href="' . $chrome . '">Google Chrome 30+</a>';
			return $text;
		} // switch
		return null;
	} //observe()

} //class Menu

//Register this class as an Observer
Plugins::register(Plugins::OBSERVER, 'Browsers');
