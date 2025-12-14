<?php

/**
 * @brief Interfaces for plugins
 * @version JEWE 3.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\engine;

/**
 * @interface
 * @brief Interface for a plugin that wants to react to a specific event
 */
interface Observer {

	/**
	 * @brief  returns an array of events which the plugin wants to observe
	 * @return int[]
	 */
	public static function getEvents() : array;


	/**
	 * @brief  handles the event
	 * @param  int $event event that just occurred
	 * @param  array|null $params null or any params of the event
	 * @return string|null null or String as a response
	 */
	public function observe(int $event, ?array $params) : ?string;
}
