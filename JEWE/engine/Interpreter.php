<?php

declare(strict_types=1);

namespace JEWE\engine;

/**
 * @interface
 * @brief Interface for a plugin that is able to handle one-line command
 */
interface Interpreter {

	/**
	 * @brief  returns an array of keywords which the plugin is able to handle
	 * @param  [void]
	 * @return string[]
	 */
	public static function getCommands() : array;


	/**
	 * @brief  handles the command
	 * @param  string $command command that raised this event
	 * @param  string $params params after the keyword
	 * @return string|null Null or String which to put in place of command
	 */
	public function execute(string $command, string $params) : ?string;
}
