<?php
$target_dir = "./uploads/";
$target_file = $target_dir . basename($_FILES["file"]["name"]);

// Check if the form was submitted and if there are no errors in the file upload
if (isset($_POST["submit"])) {
    $uploadError = $_FILES["file"]["error"]; // Get the error code

    if ($uploadError === UPLOAD_ERR_OK) {
        // File upload was successful
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
            // Write content to the uploaded file
            $content_to_write = "This is some example content that will be written to the uploaded file.\n";
            
            // Open the file for writing (you can choose different modes like "w" for overwrite or "a" for append)
            $file_handle = fopen($target_file, "a"); // "a" mode appends content

            if ($file_handle !== false) {
                // Write the content to the file
                fwrite($file_handle, $content_to_write);
                
                // Close the file handle
                fclose($file_handle);
                
                echo "File upload and content writing successful.";
            } else {
                echo "Sorry, there was an error opening the file for writing.";
            }
        } else {
            echo "Sorry, there was an error uploading your file.";
        }
    } else {
        // Print the specific error code
        echo "Sorry, there was an error in the file upload. Error code: " . $uploadError;
    }
}
?>
