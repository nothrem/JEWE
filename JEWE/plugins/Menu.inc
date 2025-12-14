<?php

/**
 * @brief JEWE plugin that creates menu
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */

namespace JEWE\plugins;

use JEWE\engine\Config;
use JEWE\engine\Events;
use JEWE\engine\Plugin;
use JEWE\engine\Observer;
use JEWE\engine\Plugins;
use JEWE\plugins\Chapters\Chapter;

/**
 * @brief plugin class that creates menu
 */
class Menu implements Plugin, Observer {
	public static function getVersion() : float
	{
		return 1.0;
	}


	public function init(int $kind) : bool
	{
		//menu requires plugin Chapters
//		return Plugins::isPluginRegistered(Plugins::INTERPRETER, 'Chapters');
		return true;
	}


	public static function getEvents() : array
	{
		return [
			Events::ON_HEADER,
			Events::ON_BODY_START,
			Events::ON_BODY_END,
		];
	}


	public function observe($event, $params) : ?string
	{
		$title = Config::get('title');
		$logo = Config::get('logo');
		$logoW = Config::get('logoWidth');
		$logoH = Config::get('logoHeight');
		$skip = Config::get('skipFirstChapter');

		switch ($event) {
			case Events::ON_HEADER :
				return '<script src="' . 'JEWE/plugins/Menu/createMenu.js" type="text/javascript" language="JavaScript"></script>';
			case Events::ON_BODY_START :
				return '<div id="top"><a name="top">' .
				        //title for printing
				        '<h1 id="print_logo">' . $title . '</h1>'
				        //logo
				        . '<img onload="menu.fitImage()" id="logo" src="' . $logo . '" alt="' . $title . '" title="' . $title
				        . '" border="0" height="' . $logoH . '" width="' . $logoW . '">'
				        //menu
				        . '<a name="topmenu"><div id="menu">Čekejte, načítám menu...</div></a>'
				        //copyright
				        . '<p id="copyright"><a href="http://jewe.nothrem.cz">JEWE</a> by Nothrem Sinsky &copy; 2006-2025</p>'
				        //end of div 'top'
				        . '</a></div>';
			case Events::ON_BODY_END :
				$js = '<script type="text/javascript" language="JavaScript">';
				$js .= "\n";
				//get all chapters (from plugin Chapters)
				/** @var Chapters $chapters */
				$chapters = Plugins::getPlugin(Plugins::INTERPRETER, Chapters::class);
				$chapters = $chapters->root;
				$js .= 'menu.skipFirstChapter = ' . ($skip ? 'true' : 'false') . ';' . "\n";
				$js .= 'menu.showChapterIndex = ' . (Config::get('showChapterIndex') ? 'true' : 'false') . ';' . "\n";
				//$js .= 'menu.chapters.mainmenu = ';
				//$js .= $this->convertChapterToJs($chapters);
				//$js .= ';'."\n";
				//$js .= 'menu.create("mainmenu");';
				$js .= "\n";
				$js .= '</script>' . "\n";
				$js .= '<div id="menuStore" style="display:none"';
				//$js .= ' onload="menu.switchTo(\'mainmenu\', \'menu\');"';
				$js .= '>' . "\n";
				$js .= $this->convertChapterToMenu($chapters, 'mainmenu');
				$js .= '</div>' . "\n";
				$js .= '<script type="text/javascript" language="JavaScript">';
				$js .= 'menu.switchTo(\'mainmenu\', \'menu\');';
				$js .= '</script>' . "\n";
				return $js;
		} // switch

		return null;
	}


	private function convertChapterToMenu($chapter, $index, $parent = null) : string
	{
		$skip = Config::get('skipFirstChapter');
		$skip = !isset($parent) && $skip; //only the first level index can be skipped
		$order = Config::get('showChapterIndex');
		$menu = '<ul id="menuStore_' . $index . '" class="menu" style="display:none">' . "\n";
		if (isset($parent)) {
			$menu .= '<li class="back"><a href="#' . $parent . '" onclick="menu.switchTo(\'' . $parent
			         . '\', \'menu\'); return false;">&lt;= Zpět do předchozího menu</a></li>';
		}
		$menu .= '<li class="header">';
		if (isset($parent)) {
			$menu .= '<a href="#' . $index . '">';
		}
		$menu .= str_replace(' ', '&nbsp;', $chapter->caption);
		if (isset($parent)) {
			$menu .= '</a>';
		}
		$menu .= '</li>';
		$menu .= "\n";
//		echo $chapter->caption, ' has ', count($chapter->chapters), ' subchapters' . "\n";
		foreach ($chapter->chapters as $subindex => $subchapter) {
			if (!$skip) {
				$subindex++;
			}
			$id = $index . '_' . $subindex;
			$menu .= '<li _id="' . $id . '" class="menuItem">';
			$menu .= '<a href="#' . $id . '"';
			if (!$subchapter->isEmpty()) {
				$menu .= ' onclick="menu.switchTo(\'' . $id . '\', \'menu\'); return false;">';
			}
			else {
				$menu .= '>';
			}
			if ($order && (!$skip || 0 < $subindex)) {
				$menu .= $subindex . '.&nbsp;';
			}
			$menu .= str_replace(' ', '&nbsp;', $subchapter->caption) . '</a></li>' . "\n";
			if (!$subchapter->isEmpty()) {
				$menu .= $this->convertChapterToMenu($subchapter, $index . '_' . $subindex, $index);
			}
		}
		$menu .= '</ul>' . "\n";
		return $menu;
	}


	private function convertChapterToJs($chapter) : string
	{
		$js = '{';
		$js .= 'caption: "' . $chapter->getCaption() . '",';
		$js .= 'items: {';
		$chapters = [];
		foreach ($chapter as $index => $subchapter) {
			$chapters[] = $index . ':' . $this->convertChapterToJs($subchapter);
		}
		$js .= implode(',', $chapters);
		$js .= '}';
		$js .= '}';
		return $js;
	}
} //class Menu

//Register this class as an Observer
Plugins::register(Plugins::OBSERVER, 'Menu');
