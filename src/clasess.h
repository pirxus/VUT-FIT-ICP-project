class Map {
	/*
	seznam křižovatek, seznam ulic 
	křižovatka - seznam ulic, který vedou do té křižovatky, souřadnice
	setPositions
	*/
}

class Street {
	/*
	začátek, konec, seznam zastávek, jméno ulice
	stupeň provozu
	setTrafficPace, setStart, setStops, getName 
	*/
}

class Stop {
	/*
	ukazatel na ulici, souřadnice, jméno zastávky
	getPosition, setPosition, 
	*/
}

class Line {
	/*
	seznam zastávek, seznam spojů, barva
	getSchedule, setCon, setStops, setColor
	*/
}

class Con{
	/*
	jízdní řád - seznam časů, pořadí časů - pořadí zastávek
	ukazatel na linku
	aktuální zpoždění
	seznam křižovatek mezi dvěma zastávkami
	getPosition, setCrossroads, countDelay, getSchedule
	*/
}

class Time {
	/*
	time, rychlost, default = 1
	setTime, setSpeed, 
	*/
}

class MHD {
	/*
	Mapa, seznam linek, čas, 
	loadMap, loadLines, setTime, getPositions, 
	*/
}	

class Crossroads {
	/*
	x,y, seznam ulic
	*/	
}