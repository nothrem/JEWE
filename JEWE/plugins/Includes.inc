<?php

/**
 * @brief JEWE plugin that handles Included files
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Plugins;
use RuntimeException;

/**
 * @brief plugin class that handles chapters1
 */
class Includes implements Plugin, Interpreter {

	/**
	 * stores list of included files
	 */
	private array $includes;

	/**
	 * stores reference to used parser
	 */
	private HtmlParser $parser;


	public static function getVersion() : float
	{
		return 1.0;
	}


	public static function getCommands() : array
	{
		return ['INC', 'include', 'soubor'];
	}


	public function init(int $kind) : bool
	{
		$plugin = Plugins::getPlugin(Plugins::OBSERVER, HtmlParser::class);
		if (!$plugin instanceof HtmlParser) {
			$plugins = Plugins::getSelf()->plugins[Plugins::OBSERVER];
			throw new RuntimeException('Could not find HtmlParser plugin in ['.implode(', ', array_keys($plugins)) . ']');
		}
		$this->parser = $plugin;
		return true;
	}


	public function execute(string $command, string $params) : ?string
	{
		$fileName = trim($params);
		$fileName = getcwd() . '/' . $fileName;

		if (is_file($fileName)) {
			$file = file($fileName);
			$this->parser->process($file);
			return implode(PHP_EOL, $file);
		}

		return "<script>alert('Error: File $fileName not found!');</script>\n";
	}
}

//Register this class as Interpreter and Observer
Plugins::register(Plugins::INTERPRETER, 'Includes');
