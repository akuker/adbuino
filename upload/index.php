<?php
// Configuration
$title = 'Aquinas Router';
$filedir = 'up';
$maxsize = 5242880; //max size in bytes
$allowedExts = array('png', 'jpg', 'jpeg', 'gif');
$allowedMime = array('image/png', 'image/jpeg', 'image/pjpeg', 'image/gif');
$baseurl = $_SERVER['HTTP_HOST'].dirname($_SERVER['REQUEST_URI']).'/'.$filedir;
?>
<html>
<head>
    <title><?php print $title; ?></title>
<link rel="stylesheet" href="css.css" type="text/css" />
</head>
<body>
	<div id="upload">
		<form enctype="multipart/form-data" action="<?php print $_SERVER['PHP_SELF']; ?>" method="POST">
		<input type="hidden" name="MAX_FILE_SIZE" value="5242880" />
		Choose a file to upload: <br />
		<input size="62"  name="file" type="file" accept="image/*" />
		<input type="submit" value="Upload File" />
		</form>
		<div id="info">
		Max file size: 5mb <br/>
		Supported formats: png, jpg, gif <br/>
		At the sides of this room are the channels that have gone dark. <br/> Open them, yes. Wideband. Ultrawhite. Yes...
		</div>
	</div>
	<div id="image">
	<a name="image">
<?php
	if($_SERVER['REQUEST_METHOD'] == 'POST') {
		$ext = pathinfo($_FILES['file']['name'], PATHINFO_EXTENSION);

		if ((in_array($_FILES['file']['type'], $allowedMime))
		&& (in_array(strtolower($ext), $allowedExts)) 
		&& (@getimagesize($_FILES['file']['tmp_name']) !== false)
		&& ($_FILES['file']['size'] <= $maxsize)) {
			$md5 = substr(md5_file($_FILES['file']['tmp_name']), 0, 7);
			$newname = time().$md5.'.'.$ext;
			move_uploaded_file($_FILES['file']['tmp_name'], $filedir.'/'.$newname);
			$baseurl = $_SERVER['HTTP_HOST'].dirname($_SERVER['REQUEST_URI']).'/'.$filedir;
			$imgurl = 'http://'.$baseurl.'/'.$newname;
			print '<br />';
			print 'Your URL:<br />';
			print '<input type="text" value="'.$imgurl.'" ><br /><br />';
			print '<a href="'.$imgurl.'"><img src="'.$imgurl.'" /></a><br />';
		}

		else {
			print '<br />';
			print 'Something went wrong <br />';
		}
		
	}
?>
	</div>
</body>
</html>

