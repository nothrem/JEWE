<?php

/**
 * @brief static class that offers interface to access configuration
 * @version JEWE 3.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinski
 */

namespace JEWE\engine;

/**
 * @singleton
 * @brief  Interface for JEWE configuration
 */
final class Config extends Singleton {

	private static string $config = 'config';


	public static function get($property) : mixed
	{
		$name = self::$config;
		$config = new $name();

		if (property_exists($config, $property)) {
			return $config->$property;
		}

//		throw new Exception('Property "'.$property.'" not found in JEWE configuration!');
		return null;
	} //get()

} //class Config
