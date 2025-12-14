<?php

/**
 * @brief static class for event constants
 * @version JEWE 3.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\engine;

/**
 * Class with event constants
 */
final class Events {
	/** reserved*/
	public const int OTHER = 0;
	/**
	 * events for JEWE execution
	 * params: none
	 * return: String
	 */
	public const int ON_START = 11;
	/**
	 * events for position in file
	 * params: none
	 * return: String
	 */
	public const int ON_HEADER = 12;
	public const int ON_BODY_START = 13;
	public const int ON_BODY_END = 14;


	/**
	 * @brief  rises event and notifies all observers
	 * @param  $event [Integer] event to rise
	 * @param  $params [Array] params for the event
	 * @return string[] results from all observers
	 */
	public static function rise(int $event, ?array $params = null) : array
	{
		/** @var Plugins $plugins */
		$plugins = Singleton::getInstance(Plugins::class);
		return $plugins->riseEvent($event, $params);
	}

	public static function start() : array {
		return self::rise(self::ON_START);
	}
}
