/**
 * @brief script to generate menu
 * @version 1.0
 * @author Nothrem Sinsky <jewe@nothrem.cz>
 * @copyright 2008-2025 Nothrem Sinsky
 */
var menu = {
	/**
	 * Creates new menu from chapters
	 * @param  chapter [String] name of the chapter of chapters property
	 * @return [void]
	 */
	create: function(chapter) {
		var item = this.chapters[chapter];

		var html = '', subMenu = '', skip = this.skipFirstChapter;
		var caption = this.chapters[chapter].caption;
		if ('' !== caption) {
			html += '<span class="menuCaption">' + caption + '</span>';
		} 
		html += '<ul id="menu_' + chapter + '" class="menu ' + chapter + '">';
		var subChapter, subName;
		for (var i in item.items) {
			subChapter = item.items[i];
			if (!skip) {
				i++;
			}
			subName = chapter + '_' + i;
			//create chapter from own subitems
			if (undefined !== subChapter.items[0]) {
				this.chapters[subName] = subChapter;
			}	
			html += '<li id="menuItem_' + subName + '" class="menuItem">';
			html += '<a href="#' + subName +'"';
			if (undefined !== this.chapters[subName]) {
				html += ' onclick="menu.create(\"'+ subName + '\"); return false;"';
			}
			html += '>';
			if (this.showChapterIndex) {
			 	html += (0 !== i?i + '.&nbsp;':'');
			}
			html += subChapter.caption;
			html += '</a></li>';
			subMenu += '<div id="' + subName + '">&nbsp;</div>';
		} //each chapter
		html += '</ul>';
		html += subMenu;
		var div = document.getElementById(chapter);
		if (div) {
			div.innerHTML = html;
		}
		else {
			alert('Menu nenalezeno!');
		}
	}, //createMenu()
	
	/**
	 * @brief  switches menu to another one
	 * @param  menuName [String] id of the DOM element the menu is in
	 * @param  placeInto [String] id of the DOM element where to place the menu
	 * @return [void]
	 * @note   content of the 'placeInto' element will be moved into 'menuStore' element 
	 */
	switchTo: function(menuName, placeInto) {
		var menuStore = document.getElementById("menuStore");
		var menuNode  = document.getElementById("menuStore_"+menuName);
		var place     = document.getElementById(placeInto);
		if (place) { //check the place exists
			if (menuStore) { //move old menu into store
				while (place.firstChild) {
					if (place.firstChild.style) {
						place.firstChild.style.display = 'none';
					}
					menuStore.appendChild(place.firstChild);
				}
			}
			else {
				alert('Cannot find menuStore to put away old menu!');
			}
			//move menu into place
			if (menuNode) {
				place.appendChild(menuNode);
				menuNode.style.display = '';
			}
			else {
				alert('Cannot find menu ' + menuName + '!');
			}
		}
		else {
			alert('Cannot find element '+ placeInto +' to put menu into.');			
		}
	},
	
	chapters: {},
	
	fitImage: function() {
  
  }
}; //menu
