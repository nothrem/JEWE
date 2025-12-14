<?php

/**
 * @brief class for text parsing
 * @version 1.1
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

/**
 * History:
 * 1.0
 * + basic version
 * + parseFile() to create HTML file
 * + process() to parse lines and execute commands
 *
 * 1.1
 * + process() with commandMap to change scope
 *
 */

namespace JEWE\plugins;

use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;
use JEWE\engine\Config;
use RuntimeException;

/**
 * Class for parsing a text file
 */
class HtmlParser implements Plugin, Observer {
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
		return [Events::ON_START];
	}


	public function observe($event, $params) : ?string
	{
		if ($event === Events::ON_START) {
			$file = Config::get('startPage');
			$file = getcwd() . '/' . $file;

			if (is_file($file)) {
				return $this->parseFile($file);
			}

			return "<script>alert('" . tr("Error: Content file not found! Page loading halted.") . "');</script>\n";
		} // switch

		return null;
	}


	/**
	 * Opens file and parses it for commands
	 */
	public function parseFile(string $filename) : string
	{
		//first create the head of the HTML file
		$lines = Events::rise(Events::ON_HEADER);
		$head = implode(PHP_EOL, $lines);

		//now create HTML body
		$lines = Events::rise(Events::ON_BODY_START);
		$body = implode(PHP_EOL, $lines);

		//parse file
		$file = file($filename);
		$this->process($file);
		$body .= implode(PHP_EOL, $file);

		//get end of the body
		$lines = Events::rise(Events::ON_BODY_END);
		$body .= implode(PHP_EOL, $lines);

		$lang = Config::get('language') ?? 'en';

		//create the final HTML file
		return <<<HTML
<!DOCTYPE html>
<html lang="$lang">
<head>
$head
</head>
<body>
$body
</body>
</html>
HTML;
	} //parseFile()


	/**
	 * @brief  processes lines and executes commands
	 * @param  $lines [&Array of Strings] lines to process
	 * @param  $commandMap [Array of String] if command found as index, value will be uses instead
	 * @return [void]
	 */
	public function process(&$lines, $commandMap = [])
	{
		$emptyLine = false;
		foreach ($lines as $row => $line) {
			$trim = trim($line);
			if (preg_match('/^\#([^\ ]+)(\ (.*))?$/', $trim, $params)) {
				$command = $params[1];
				$params = array_key_exists(3, $params) ? $params[3] : '';
				if (array_key_exists($command, $commandMap)) {
					$params = $command . ' ' . $params;
					$command = $commandMap[$command];
				}
				$result = Plugins::executeCommand($command, $params);
				if (false === $result) {
					echo 'Cannot process command "', $command, '" with parameters "', $params, '"', PHP_EOL;
					throw new RuntimeException('Unknown command "' . $command . '"');
				}

				$lines[$row] = $result;
			}
			else { //line w/o command
				if ($emptyLine and preg_match('/^[^\<](.*)?$/', $trim, $params)) {
					//paragraph following after empty line is converted into HTML paragraph
					$lines[$row] = '<p>' . $trim . '</p>';
				}
				$emptyLine = ("" === $trim);
			}
		} //foreach(line in file)
	}
}

//Register this class as Interpreter and Observer
Plugins::register(Plugins::OBSERVER, 'HtmlParser');
