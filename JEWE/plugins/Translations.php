<?php

/**
 * @brief class for text translation
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

declare(strict_types=1);

namespace JEWE\plugins;

use JEWE\engine\Events;
use JEWE\engine\Observer;
use JEWE\engine\Plugin;
use JEWE\engine\Config;

/**
 * @brief plugin class that creates tr() method
 */
class Translations implements Plugin, Observer {

	/**
	 * Stores translations for PHP plugins
	 */
	private array $trCache = [];
	private string $language = 'en';


	public static function getVersion() : float
	{
		return 1.0;
	}


	public function init(int $kind) : bool
	{
		$this->language = Config::get('language') ?? 'en';
		$fileName = 'translations/' . $this->language . '.inc';
		$fileName = getcwd() . '/' . $fileName;

		if (!Config::get('translate')) {
			return true; //initialized, but without translations
		}

		if (is_file($fileName)) {
			include($fileName);
		}
		else {
			echo "<script>alert('Error: File $fileName not found!');</script>\n";
		}

		return true;
	}


	public static function getEvents() : array
	{
		return [
			Events::ON_HEADER,
		];
	}


	public function observe(int $event, ?array $params) : ?string
	{
		if ($event === Events::ON_HEADER) {
			return '<script src="' . __DIR__ . 'Translation/tr.js" type="text/javascript" language="JavaScript"></script>';
		}

		return null;
	}

}
