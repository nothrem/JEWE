<?php

/**
 * @brief JEWE plugin that handles keywords for game Mass Effect
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Config;
use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;

/**
 * @brief plugin class that handles keywords for game Mass Effect
 */
class TombRaider implements Plugin, Interpreter, Observer {

	private HtmlParser $parser;
	private $includeIndex;


	public static function getVersion() : float
	{
		return 1.0;
	} //getVersion()


	public static function getCommands() : array
	{
		return [
			'secret',
			'spoiler',
			'unlock', //obecné
			'artefakty',
			'relikvie', //secrety z TRA
			'památky',
			'pamatky', //secrety z TRU
			'bronz',
			'stříbro',
			'stribro',
			'zlato', //secrety z TRL
		];
	} //getCommands()


	public function init(int $kind) : bool
	{
		$plugin = Plugins::getPlugin(Plugins::OBSERVER, HtmlParser::class);
		if (!$plugin instanceof HtmlParser) {
			$plugins = Plugins::getSelf()->plugins[Plugins::OBSERVER];
			throw new RuntimeException('Could not find HtmlParser plugin in ['.implode(', ', array_keys($plugins)) . ']');
		}

		$this->parser = $plugin;
		$this->includeIndex = 0;
		return true;
	} //init()


	public function execute($command, $params) : ?string
	{
		return match ($command) {
			'secret' => '<p class="secret">' . $params . '</p>',
			'spoiler' => '<p class="spoiler">' . $params . '</p>',
			'artefakty' => '<p class="artefakt">Artefaktů: <em>' . $params . '</em></p>',
			'relikvie' => '<p class="relikvie">Relikvií: <em>' . $params . '</em></p>',
			'památky', 'pamatky' => '<p class="relikvie">Památek: <em>' . $params . '</em></p>',
			'bronz' => '<p class="relikvie">Bronzových odměn: <em>' . $params . '</em></p>',
			'stříbro', 'stribro' => '<p class="relikvie">Stříbrných odměn: <em>' . $params . '</em></p>',
			'zlato' => '<p class="relikvie">Zlatých odměn: <em>' . $params . '</em></p>',
			'unlock' => '<p class="artefakt">Dobráním se odemkne: <em>' . $params . '</em></p>',
			default => null,
		};
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
Plugins::register(Plugins::INTERPRETER, 'TombRaider');
Plugins::register(Plugins::OBSERVER, 'TombRaider');
