<?php

declare(strict_types=1);

namespace JEWE\engine;

/**
 * @brief Singleton parent class
 * @version JEWE 3.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */
abstract class Singleton {

	private static array $instances = [];


	/**
	 * @access protected - only this class can create instances of its child classes
	 */
	protected function __construct() {}


	protected static function createInstance($class, $params = null) : void
	{
		if (!isset(self::$instances[$class])) {
			self::$instances[$class] = new $class($params);
		}
	}


	/**
	 * @param  string $class
	 * @return static Instance of $class
	 */
	public static function getInstance(string $class) : static
	{
		self::createInstance($class);
		return self::$instances[$class];
	}
}
