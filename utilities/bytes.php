<?php
/*
 * Utility to geneate string of hex values for use with the Arduino ICT project https://github.com/prswan/arduino-mega-ict
 * 
 * Usage: 
 * Add this file to a new folder with any files you want to generate hex values for
 * From the command line run as follows:
 * path/to/php bytes.php
 * 
 * Example usage and Output (Mac OSX 10.11.3):
 * Name:directory user$ php bytes.php
 * File: defender.snd | CRC: 0xfefd5b48 | {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x1b,0xcb,0x5a,0x97,0xce}
 * File: stargate.snd | CRC: 0x2fcf6c4d | {0x0f,0x8e,0x7f,0x6f,0x6f,0x04,0x1b,0xcb,0x5a,0x97,0x96}
 * Done
 */
$files = scandir(".");
foreach ($files as $filename) {
	if (in_array($filename,array(".","..",".DS_Store","bytes.php"))) continue;
	$handle = fopen($filename, "rb"); 											
	$filesize = filesize($filename); 
	$byteArray = array_values(unpack("C*",fread($handle, $filesize)));
	$string = "";
	$index = 1;
	while ($index < $filesize) {
		$string .= "0x".($byteArray[$index] < 16 ? "0" : "").dechex($byteArray[$index]).',';
		$index = $index*2;
	}
	echo  "File: ".$filename." | "."CRC: 0x".dechex(crc32(file_get_contents($filename)))." | {".str_replace(",}", "}", $string."}")."\n";
}
die("Done\n");
?>
