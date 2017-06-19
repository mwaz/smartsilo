<?php
session_start();

$userinfo = array(
                'admin'=>'admin',
                'admin2'=>'admin2'
                );

if(isset($_GET['logout'])) {
    $_SESSION['username'] = '';
    header('Location:  ' . $_SERVER['PHP_SELF']);
}

if(isset($_POST['username'])) {
    if($userinfo[$_POST['username']] == $_POST['password']) {
        $_SESSION['username'] = $_POST['username'];
         header("location:http://smartsilo.mazingiraproject.com/temperature.html");
    }else {
        //Invalid Login
         header("location:http://smartsilo.mazingiraproject.com/temperature.html");
    }
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <title>Login</title>
        <link href="css/login.css" rel="stylesheet">
    </head>

    
    <body>


        <?php if($_SESSION['username']): ?>
            <p>You are logged in as <?=$_SESSION['username']?></p>
            <p><a href="?logout=1">Logout</a></p>
        <?php endif; ?>
         <div class="form">
        <form name="login" action="" method="post" class="login-form">
        <center> <strong> Smart Silo is an Agriculture Project Aiming to increase Food Security Using IoT and Sensors in silos</strong> </center>
        <br>    <br>    <br>    
            Username:  <input type="text" name="username" value="" /><br />
            Password:  <input type="password" name="password" value="" /><br />
            <input type="submit" name="submit" value="Submit" />
        </form>
    </body>
</html>









