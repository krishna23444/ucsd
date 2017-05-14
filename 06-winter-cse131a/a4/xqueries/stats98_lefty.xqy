for $p in document("stats.xml")//PLAYER
    where $p/THROWS = "Left"
	return <lefty> {$p/POSITION/text()} {" "}  {$p/SURNAME/text()} </lefty>
