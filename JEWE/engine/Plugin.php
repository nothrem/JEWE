<?php

declare(strict_types=1);

namespace JEWE\engine;

/**
 * @interface
 * @brief General Interface for plugins
 */
interface Plugin {

	/**
	 * @brief  returns version of this plugin
	 * @return float version
	 */
	public static function getVersion() : float;


	/**
	 * @brief  method that replaces the constructor
	 * @param  int $kind Type of the plugin (for those with more kinds)
	 * @return Bool False to unregister the plugin
	 */
	public function init(int $kind) : bool;
}
