<?php
	require 'general/connect.php';
	
	if(isset($_POST["name"]) && isset($_POST["score"])) {
		//ADD NEW SCORE TO BOARD
		
		$name = mysql_real_escape_string($_POST['name']);
		$score = intval($_POST["score"]);
		
		$id = 0;
		
		
		
		if(mysql_num_rows(mysql_query("SELECT id FROM LD31Scoreboard WHERE name='".$name."'"))>0 and $name!="Anonymous") {
			if($score > mysql_result(mysql_query("SELECT score FROM LD31Scoreboard WHERE name='".$name."'"), 0)) {
				mysql_query("UPDATE LD31Scoreboard SET score=$score WHERE name='".$name."'");
				
				$id = mysql_result(mysql_query("SELECT id FROM LD31Scoreboard WHERE name='".$name."'"), 0);
			}
		} else {
			$id = mysql_result(mysql_query("SELECT MAX(id) FROM LD31Scoreboard"), 0) + 1;
			
			mysql_query("INSERT INTO LD31Scoreboard (id, name, score)
						VALUES ($id, '".$name."', $score)");
			echo mysql_error();
			
			
		}
		
		$i = 1;
		$query = mysql_query("SELECT * FROM LD31Scoreboard ORDER BY score DESC");
		while($row = mysql_fetch_array($query)) {
			// Update ranks
			mysql_query("UPDATE LD31Scoreboard SET rank=$i WHERE id=".$row['id']);
			$i = $i+1;
		}
		
		$rank = mysql_result(mysql_query("SELECT rank FROM LD31Scoreboard WHERE id=$id"), 0);
		
		exit;

	} else if(isset($_POST['score'])) {
		
		$score = intval($_POST['score']);

		$query = mysql_query("SELECT rank FROM LD31Scoreboard WHERE score < $score ORDER BY score ASC LIMIT 1");
		$rows = mysql_num_rows($query);
		$newrank = 0;

		if(!$rows or $rows == 0) {
			$query = mysql_query("SELECT MAX(rank) AS maxn FROM LD31Scoreboard");
			$newrank = 1;
		}
		
		$temp = mysql_fetch_assoc($query);
		if(intval($temp['maxn']) == 0)
			$newrank = 1;

		$newrank = $newrank + intval($temp['maxn']);

		echo $newrank;
		exit;

	} else {
		?>
		<style>
			.CSSTableGenerator {
	/*margin:0px;padding:0px;*/
	margin-left: 100px;
	margin-top: 50px;
	
	display:inline-block;
	
	box-shadow: 10px 10px 5px #888888;
	border:1px solid #000000;
	
	-moz-border-radius-bottomleft:0px;
	-webkit-border-bottom-left-radius:0px;
	border-bottom-left-radius:0px;
	
	-moz-border-radius-bottomright:0px;
	-webkit-border-bottom-right-radius:0px;
	border-bottom-right-radius:0px;
	
	-moz-border-radius-topright:0px;
	-webkit-border-top-right-radius:0px;
	border-top-right-radius:0px;
	
	-moz-border-radius-topleft:0px;
	-webkit-border-top-left-radius:0px;
	border-top-left-radius:0px;
}.CSSTableGenerator table{
    border-collapse: collapse;
        border-spacing: 0;
	width:100%;
	/*height:100%;*/
	margin:0px;padding:0px;
}.CSSTableGenerator tr:last-child td:last-child {
	-moz-border-radius-bottomright:0px;
	-webkit-border-bottom-right-radius:0px;
	border-bottom-right-radius:0px;
}
.CSSTableGenerator table tr:first-child td:first-child {
	-moz-border-radius-topleft:0px;
	-webkit-border-top-left-radius:0px;
	border-top-left-radius:0px;
}
.CSSTableGenerator table tr:first-child td:last-child {
	-moz-border-radius-topright:0px;
	-webkit-border-top-right-radius:0px;
	border-top-right-radius:0px;
}.CSSTableGenerator tr:last-child td:first-child{
	-moz-border-radius-bottomleft:0px;
	-webkit-border-bottom-left-radius:0px;
	border-bottom-left-radius:0px;
}.CSSTableGenerator tr:hover td{
	
}
.CSSTableGenerator tr:nth-child(odd){ background-color:#56cfff; }
.CSSTableGenerator tr:nth-child(even)    { background-color:#ffffff; }.CSSTableGenerator td{
	vertical-align:middle;
	
	
	border:1px solid #000000;
	border-width:0px 1px 1px 0px;
	text-align:left;
	padding:7px;
	font-size:14px;
	font-family:Arial;
	font-weight:bold;
	color:#000000;
}.CSSTableGenerator tr:last-child td{
	border-width:0px 1px 0px 0px;
}.CSSTableGenerator tr td:last-child{
	border-width:0px 0px 1px 0px;
}.CSSTableGenerator tr:last-child td:last-child{
	border-width:0px 0px 0px 0px;
}
.CSSTableGenerator tr:first-child td{
		background:-o-linear-gradient(bottom, #2200ff 5%, #0061bc 100%);	background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #2200ff), color-stop(1, #0061bc) );
	background:-moz-linear-gradient( center top, #2200ff 5%, #0061bc 100% );
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#2200ff", endColorstr="#0061bc");	background: -o-linear-gradient(top,#2200ff,0061bc);

	background-color:#2200ff;
	border:0px solid #000000;
	text-align:center;
	border-width:0px 0px 1px 1px;
	font-size:17px;
	font-family:Arial;
	font-weight:bold;
	color:#ffffff;
}
.CSSTableGenerator tr:first-child:hover td{
	background:-o-linear-gradient(bottom, #2200ff 5%, #0061bc 100%);	background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #2200ff), color-stop(1, #0061bc) );
	background:-moz-linear-gradient( center top, #2200ff 5%, #0061bc 100% );
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#2200ff", endColorstr="#0061bc");	background: -o-linear-gradient(top,#2200ff,0061bc);

	background-color:#2200ff;
}
.CSSTableGenerator tr:first-child td:first-child{
	border-width:0px 0px 1px 0px;
}
.CSSTableGenerator tr:first-child td:last-child{
	border-width:0px 0px 1px 1px;
}
		</style>

		<?
		echo "<h1 style='font-family:Arial;'>Owl'd Hurricrash scoreboard:</h1>";
		
		$tablestring = "<div style='width:1280px;height:600px;background-image: url(\"http://ludumdare.com/compo/wp-content/compo2/407933/8838-shot0.png\");'><div class='CSSTableGenerator'><table><tbody><tr><td>Rank</td><td>Name</td><td>score</td></tr>";
		
		$query = mysql_query("SELECT * FROM LD31Scoreboard ORDER BY rank ASC");
		while($row = mysql_fetch_array($query)) {
			$tablestring = $tablestring . "<tr><td>".$row['rank']."</td><td>".$row['name']."</td><td>".$row['score']."</td></tr>";
		}
		
		$tablestring = $tablestring . "</tbody></table></div></div>";
		
		echo $tablestring;
	}

?>
