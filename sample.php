<?php
// This is a comment

/*
   Multi-line comment
   to be removed by the lexer
*/

define ("MAX_LIMIT", 100) ; 

$number = 42 ;
$price = 99.99 ;
$name = "John Doe" ;

function add($a , $b) {
    $sum = $a + $b ;
    return $sum ;
}

function displayMessage($msg) {
    echo "Message: " . $msg ;
}

$result = add(10, 20);
displayMessage("Hello, PHP!") ;
?>
