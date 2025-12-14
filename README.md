# JEWE

Just Edit Walkthrough Engine

## What is JEWE?

JEWE is a PHP engine _(for generating HTML with CSS and JS)_ for easier writing of game walkthroughs. It is not a classic _content management system_ but rather a set of files that can be brought to the desired state and functionality through simple editing.

The idea was based on a project for generating PDF from files that resembled the current version of JEWE, which I worked on in 2003 for the University of West Bohemia.

## Installation

1. Download the JEWE folder and place it in the project folder
2. Download index.php and config.php and place them in the project folder
3. Insert or create an HTML file that you want to process and enter its name in config.php

## History

### version HL2:EO 0.1 (2006)

(this was not yet JEWE, but only a simple engine for <a href="http://half-life.nothrem.cz/e1/">walkthrough for the game Half-Life 2: Episode One</a>)

+ automatic insertion and formatting of images according to the #IMG keyword line
+ basic styles and formatting

### version JEWE 1.0 (2007; first version)

+ JEWE is an acronym for 'Jade Empire Walkthrough Engine' - created as an engine for <a href="http://jade.nothrem.cz/">walkthrough for the game Jade Empire</a>
+ improved work with images
+ automatic formatting of headings according to the #kapitola (chapter) and #úkol (task) keyword lines
+ automatic menu generation and dynamic changes according to chapters and tasks

### version JEWE 1.1

+ version used for <a href="http://tra.nothrem.cz/">walkthrough for Tomb Raider Anniversary</a> (preserving name and appearance)
+ ability to recognize and format arbitrary keywords (except images and chapters)
+ the menu changed to two-column

### version JEWE 1.2

+ version used for <a href="http://bioshock.nothrem.cz/">walkthrough for the game Bioshock</a> and <a href="http://half-life.nothrem.cz/e2/">walkthrough for the game Half-Life2: Episode Two</a>
+ menu again two-row
+ simplified graphics

### version JEWE 2.0 (2008)

+ completely new processing
+ plugin-based engine (the engine itself contains only minimal functionality)
+ plugin types
    - Interpreter (processing of keywords and translation to HTML)
    - Observer (reaction to given events)
        * on start (announces the actual beginning of document processing)
        * on header (triggered when processing HTML header)
        * on body start (triggered before document processing)
        * on body end (triggered before the end of document processing)

+ plugins
    + Parser (loading page content from configuration and parsing it)
    + Title (generating page title from configuration)
    + Chapters (managing document chapters)
    + Menu (generating menu from chapters)
    + Styles (automatic loading of CSS styles)
    + Images (functionality of original JEWE engine for inserting images)
    + Browsers (displaying supported browsers - IE7, FF2, FF3, Opera9.5, Safari3)
    + TombRaider (handling of formating for keywords used for Tomb Raider walkthrough)

## version JEWE 2.1 (2010)
+ version used for <a href="http://masseffect.nothrem.cz/">walkthrough for the game series Mass Effect</a>
+ new plugins
  + Includes (allows to split text into multiple files)
  + Notes (separate formating for notes)
  + MassEffect (formating for keywords used for Mass Effect walkthrough)

## version JEWE 2.2 (2016)
+ version used for <a href="http://7magu.nothrem.cz/">walkthrough for the game 7 Mages</a>
+ Plugins
  + Chapters (added support for keywords in English instead of Czech)

### version JEWE 3.0 (2025)

+ code updated for PHP 8.5
+ support for PHP namespaces
+ HTML code updated to HTML5 (supported browsers changed to Edge, Safari 5+, Firefox 5+, Chrome 30+, Opera 11+)
+ code published on GitHub
+ helpers (providing functionality for other plugins)
    + GetFile (loading files from the project folder)
    + Styles (including CSS styles from plugin's folder into the HTML page)

### version JEWE 3.1 (2026)

+ support for row (v2.0) and column (v1.1) menu
+ support for graphics for page framing (v1.0/v1.1)

## Future Versions

### Plan

* plugin types
    - line-based (processing each line)

* plugins
    - tooltips (designated words display description on mouse hover)
    - links (words in text can link to other parts)

* engine features
    * caching (page goes through the script only once, then is saved to TEMP)
    * autoload (loading parts of walkthrough online - less overhead, faster download)
