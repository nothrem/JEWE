<?php

/**
 * @brief JEWE plugin that loads CSS styles
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;
use JEWE\helpers\GetFiles;

/**
 * @brief plugin class that loads CSS styles
 */
class Styles implements Plugin, Observer {
	use GetFiles;

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
		if ($event === Events::ON_HEADER) {
			$lines = self::getClassStyles(self::class);
			return implode("\n", $lines);
		} // switch

		return null;
	}


	public static function getClassStyles(string $class) : array
	{
		$styles = self::getFiles($class, __DIR__);
		$lines = [];
		foreach ($styles as $style) {
			if (is_dir($style)) {
				continue; //skip folders
			}
			$lines[] = '<link rel="stylesheet" href="' . 'JEWE/plugins' . '/' . $style . '" type="text/css">';
		}
		return $lines;
	}

} //class Styles

//Register this class as an Observer
Plugins::register(Plugins::OBSERVER, 'Styles');
