<?php 
    // Start MySQL Connection
    include('dbcon.php'); 
?>

<html>
<head>
 <meta http-equiv="refresh" content="70">
    <title>Arduino Temperature Log</title>
    <style type="text/css">
        .table_titles, .table_cells_odd, .table_cells_even {
                padding-right: 20px;
                padding-left: 20px;
                color: #000;
        }
        .table_titles {
            color: #FFF;
            background-color: #666;
        }
        .table_cells_odd {
            background-color: #CCC;
        }
        .table_cells_even {
            background-color: #FAFAFA;
        }
        table {
            border: 2px solid #333;
        }
        body { font-family: "Trebuchet MS", Arial; }
    </style>
</head>

    <body>
        <h1>Battery Voltage Log</h1>
    <table border="0" cellspacing="0" cellpadding="4" id="table_id">
      <tr>
            <td class="table_titles">ID</td>
            <td class="table_titles">Location</td>
            <td class="table_titles">Voltage</td>
          </tr>
<?php
    // $val1="10.";
    // $val2="11.";
    // $val3="12.";
    // $val4="13.";
    // $val5="14.";
    // $val6="15.";
    // $val7="16.";
    // Retrieve all records and display them
    $sql = "SELECT * FROM voltage ORDER BY id ASC";
    $query = mysqli_query($con, $sql);

    // Used for row color toggle
    $oddrow = true;

    // process every record
    while( $row = mysqli_fetch_array($query) )
    {
        if ($oddrow) 
        { 
            $css_class=' class="table_cells_odd"'; 
        }
        else
        { 
            $css_class=' class="table_cells_even"'; 
        }

        $oddrow = !$oddrow;

        echo '<tr>';
        echo '   <td'.$css_class.'>'.$row["id"].'</td>';
        echo '   <td'.$css_class.'>'.$row["battery"].'</td>';
        //echo '   <td'.$css_class.'>'.$row["voltage"].'</td>';
        
        if($row['voltage']<11) 
         echo "<td style='background-color: #D32F2F;'>".$row['voltage']."</td>"; // red
        elseif(($row['voltage']>=11)&&($row['voltage']<12))
         echo "<td style='background-color: #FFEB3B;'>".$row['voltage']."</td>"; //yellow
        elseif(($row['voltage']>=12)&&($row['voltage']<13))
         echo "<td style='background-color: #42A5F5;'>".$row['voltage']."</td>"; //blue
        elseif($row['voltage']>=13)
         echo "<td style='background-color: #4CAF50;'>".$row['voltage']."</td>"; //green
        else
         echo "<td style='background-color: white;'>".$row['voltage']."</td>";
    }
?>
    </table>
    </body>
</html>