<?php

/**
 * @brief JEWE plugin that handles notes
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Plugins;
use JEWE\engine\Observer;

/**
 * @brief plugin class that handles notes
 */
class Notes implements Plugin, Interpreter, Observer {

	public static function getVersion() : float
	{
		return 1.0;
	} //getVersion()


	public static function getCommands() : array
	{
		return [
			'note',
			'poznámka',
			'poznamka',
		];
	} //getCommands()


	public function init(int $kind) : bool
	{
		return true;
	} //init()


	public function execute($command, $params) : ?string
	{
		return '<p class="note">' . $params . '</p>';
	} //execute()


	public static function getEvents() : array
	{
		return [
			Events::ON_HEADER,
		];
	}


	public function observe($event, $params) : ?string
	{
		if ($event === Events::ON_HEADER) {
			$lines = Styles::getClassStyles(self::class);
			return implode("\n", $lines);
		} // switch

		return null;
	}

} //class Chapters

//Register this class as Interpreter and Observer
Plugins::register(Plugins::INTERPRETER, 'Notes');
Plugins::register(Plugins::OBSERVER, 'Notes');
