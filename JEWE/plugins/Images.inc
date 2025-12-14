<?php


/**
 * @brief JEWE plugin that handles Images
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

/*

Requires configuration value $imageDefinition with a path of the INI file

This INI must be in format:

[1]
name = "Name of the image"
desc = "Description of the image"
img  = "image_filename_and_path"
tmb  = "thumbnail_filename_and_path"
alt  = "alternative_image_filename_and_path"
altn = "Name of the alternative image"

*/

declare(strict_types=1);
namespace JEWE\plugins;

use JEWE\engine\Config;
use JEWE\engine\Plugin;
use JEWE\engine\Interpreter;
use JEWE\engine\Plugins;

/**
 * @brief plugin class that handles chapters1
 */
class Images implements Plugin, Interpreter {
	private const string ALIGN_RIGHT = 'right';
	private const string ALIGN_LEFT = 'left';

	/**
	 * stores image definitions
	 */
	private array $images;

	/**
	 * stores current align for the image
	 */
	private $align;


	/**
	 * @brief  returns version of this plugin
	 * @return [float] version
	 */
	public static function getVersion() : float
	{
		return 1.0;
	}


	/**
	 * @brief  returns array of keywords which the plugin is able to handle
	 * @param  [void]
	 * @return [Array of strings]
	 */
	public static function getCommands() : array
	{
		return ['IMG', 'image', 'obrázek'];
	}


	/**
	 * @brief  initiates the plugin
	 * @param  int $kind [Integer]
	 * @return [void]
	 */
	public function init(int $kind) : bool
	{
		$ini = Config::get('imageDefinition');
		if ($ini && file_exists($ini)) {
			$this->images = parse_ini_file($ini, true);
			$this->align = self::ALIGN_LEFT;
			return true;
		}

		return false;
	}


	/**
	 * @brief  switches current align
	 */
	private function switchAlign() : void
	{
		$this->align = ($this->align === self::ALIGN_LEFT) ? self::ALIGN_RIGHT : self::ALIGN_LEFT;
	}


	public function execute($command, $params) : ?string
	{
		$img = (int)$params;  //cuts leading zeros and other unwanted characters

		$this->switchAlign();

		$ini = $this->images[$img];
		$name = $ini['name']; //name of the image
		$desc = $ini['desc']; //description of the image
		$path = $ini['img'];  //path and file name of the file
		$thmb = $ini['tmb'];  //path and file name of the thumbnail
		$alti = $ini['alt'];  //path and file name if alternative image
		$altn = $ini['altn']; //name of the alternative image

		$alt = (isset($alti) && isset($altn))
			? '<br><a href="' . $alti . '" title="' . $altn . '" name="' . $altn . '">' . $altn . '</a>'
			: '';

		return
			'<div class="img_' . $this->align . '">'
			. '<a href="' . $path . '" title="' . $name . '" name="' . $name . '">'
			. '<img src="' . $thmb . '" alt="' . $name . '" title="' . $desc . '" name="' . $name . '" border="0">'
			. '</a><span>' . $name . $alt . '</span></div>' . "\n"
		;
	}
}

//Register this class as Interpreter and Observer
Plugins::register(Plugins::INTERPRETER, 'Images');
