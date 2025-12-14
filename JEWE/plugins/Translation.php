<?php

declare(strict_types=1);

namespace JEWE\plugins;

use JEWE\engine\Singleton;
use JEWE\engine\Plugins;

/**
 * @brief class for text translation
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */
class Translation extends Singleton {

	private array $trCache = [];


	/**
	 * @brief  adds new translations into the cache
	 * @param  $translations [Array] pairs of 'english' => 'translated' values
	 */
	public static function add($translations) : void
	{
		$class = parent::getInstance(self::class);

		foreach ($translations as $en => $tr) {
			$class->trCache[$en] = $tr;
		}
	} //add()


	/**
	 * @brief  translation method
	 * @param  string $text English text
	 * @return string Translated text or original English (if translation does not exist)
	 */
	public static function tr(string $text) : string
	{
		$class = parent::getInstance(self::class);

		if (array_key_exists($text, $class->trCache)) {
			return $class->trCache[$text];
		}

		return $text;
	} //tr()
}

/**
 * @brief  global function for translation
 * @param  string $text English text
 * @return string Translated text
 */
function tr(string $text) : string
{
	return Translation::tr($text);
}

//Register this class as an Observer
Plugins::register(Plugins::OBSERVER, 'Translations');
