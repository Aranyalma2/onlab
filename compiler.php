<?php

// JSON string
$jsonStr = '{
"boardID": "ESP32-C3",
  "libraryVer": "1.0",
  "boardsettings": {
    "HWIO": {
      "GPIO1": {
        "pin": "1",
        "uniqueID": "1",
        "mode": "1",
        "inputs": {
          "0": "BLOCK:1.0"
        }
      },
      "GPIO5": {
        "pin": "2",
        "uniqueID": "2",
        "mode": "3",
        "inputs": {
          "0": "IO:1.0"
        }
      }
    },
    "Com.ports": {
      "com1": {
        "hardware": "RS485",
        "workmode": "1",
        "baudrate": "4800"
      }
    }
  },
  "librarysettings": {
    "network": {
      "wifi": {
        "ssid": "test",
        "password": "test"
      },
      "cloud": {
        "cloud": "test.hu",
        "ntp": "ntp.test.hu"
      }
    },
    "local": {
      "device": {
        "boardname": "testName"
      },
      "localUser": {
        "uuid": "test",
        "password": "test"
      }
    }
  },
  "blocks":
    {
      "0": {
        "classID": "0",
        "uniqueID": "0",
        "inputs": {
          "0": "BLOCK:0.0",
          "1": "CONST:211",
          "2": "IO:1.0"
        }
      },
      "1": {
        "classID": "0",
        "uniqueID": "1",
        "inputs": {
          "0": "CONST:0"
        }
      }
    }
}';

// Decode the JSON into a PHP object
$jsonObj = json_decode($jsonStr);

///$blocksMetadata = json_decode($json_blockmetadata);

//$iosMetadata = json_decode($json_iometadata);

// Extract objects by category
$boardID = $jsonObj->boardID;
$libraryVer = $jsonObj->libraryVer;


$ios = $jsonObj->boardsettings->HWIO;
$blocks = $jsonObj->blocks;













///HERE START THE REAL IMPLEMENTACTION

//GLOBAL COMPILER OPTIONS
$COMPILE_ID;
if (isset($_GET['compileId'])) {
  $GLOBALS['COMPILE_ID'] = $_GET['compileId'];
} else {
  echo "Session compile error.";
  exit();
}

$projectName = "Alma";
$libraryVer = "v0.0.1";
$boardId = "atmega328";
$boardFQBN = "arduino:avr:nano:cpu=atmega328";
$platform = "arduino.avr.nano";

//LOAD libary metadata jsons
$libaryMetaDataPath = "arduino_cli/libraries/" . $boardId . "/" . $libraryVer . "/metadata/";

$blocksMetadata = json_decode(file_get_contents($libaryMetaDataPath . "blocks.json"));
$iosMetadata = json_decode(file_get_contents($libaryMetaDataPath . "ios.json"));

//--------------RAW CPP CODE VARS STARTS HERE--------------
$semicolon_CPP = ";";
$curlybracketOPEN_CPP = "{";
$curlybracketCLOSE_CPP = "}";
$roundbracketOPEN_CPP = "(";
$roundbracketCLOSE_CPP = ")";
$arrayOperator_CPP = "->";
$dereferenceOperator = "*";
$arduinoSETUP_CPP = "void setup(){\n__content__};\n";
$arduinoLOOP_CPP = "void loop(){\n__content__};\n";
$globalBlockContainer_CPP = "LogicModule *globalBlockContainer[__size__];";
$globalBlockContainerSize_CPP = "uint16_t globalBlockContainerSize=";
$globalBlockContainerSET_CPP = "globalBlockContainer[__idx__]=";
$globalBlockContainerGET_CPP = "globalBlockContainer[__idx__]";
$globalInputContainer_CPP = "LogicModule *globalInputContainer[__size__];";
$globalInputContainerSize_CPP = "uint16_t globalInputContainerSize=";
$globalInputContainerSET_CPP = "globalInputContainer[__idx__]=";
$globalInputContainerGET_CPP = "globalInputContainer[__idx__]";
$globalOutputContainer_CPP = "LogicModule *globalOutputContainer[__size__];";
$globallOutputContainerSize_CPP = "uint16_t globalOutputContainerSize=";
$globalOutputContainerSET_CPP = "globalOutputContainer[__idx__]=";
$globalOutputContainerGET_CPP = "globalOutputContainer[__idx__]";
$setInput_CPP = "setInput(__idx__, __value__)";
$getOutput_full_CPP = "*(__source__->getOutput(__idx__))";
$runModules_CPP = "for(uint16_t i = 0; i < globalInputContainerSize; i++){globalInputContainer[i]->run();}
for(uint16_t i = 0; i < globalBlockContainerSize; i++){globalBlockContainer[i]->run();}
for(uint16_t i = 0; i < globalOutputContainerSize; i++){globalOutputContainer[i]->run();}";
//--------------- RAW CPP CODE VARS ENDS HERE---------------

//-------------CREATED CPP CODE CONTAINERS STARTS HERE-------------
//Note these arrays element is a CPP code line
$CODE_CONTAINER_includeHeaders = array();
$CODE_CONTAINER_globals = array();
$CODE_CONTAINER_setup;
$CODE_CONTAINER_loop;

$CODE_CONTAINER_inputs = array();
$CODE_CONTAINER_outputs = array();
$CODE_CONTAINER_blocks = array();
$CODE_CONTAINER_connections = array();
//--------------CREATED CPP CODE CONTAINERS ENDS HERE--------------

//-------------TMP ASSOCIACTION ARRAYS STARTS HERE-------------
//NOT USED STILL
$tmp_input_ids = array();
$tmp_output_ids = array();
$tmp_block_ids = array();
//--------------TMP ASSOCIACTION ARRAYS ENDS HERE--------------

//-------------MAIN STARTS HERE-------------

//construction main ino file from json
$newBlocks = generateBlockConstructors($jsonObj->blocks, $blocksMetadata);
createGlobalContainer($newBlocks, $GLOBALS['globalBlockContainer_CPP'], $GLOBALS['globalBlockContainerSize_CPP']);
$scheduled_blocks = runtimeSchedule($ios, $iosMetadata, $blocks, $newBlocks);
//$GLOBALS['CODE_CONTAINER_blocks'] = generateLogicModulCreation($newBlocks, $GLOBALS['globalBlockContainerSET_CPP']);
$GLOBALS['CODE_CONTAINER_blocks'] = generateLogicModulCreation($scheduled_blocks, $GLOBALS['globalBlockContainerSET_CPP']);

$iomodules = generateIOConstructors($ios, $iosMetadata);
createGlobalContainer($iomodules[0], $GLOBALS['globalInputContainer_CPP'], $GLOBALS['globalInputContainerSize_CPP']);
createGlobalContainer($iomodules[1], $GLOBALS['globalOutputContainer_CPP'], $GLOBALS['globallOutputContainerSize_CPP']);
$GLOBALS['CODE_CONTAINER_inputs'] = generateLogicModulCreation($iomodules[0], $GLOBALS['globalInputContainerSET_CPP']);
$GLOBALS['CODE_CONTAINER_outputs'] = generateLogicModulCreation($iomodules[1], $GLOBALS['globalOutputContainerSET_CPP']);

$blockconnection = generateBlocksSourceSetters($GLOBALS['CODE_CONTAINER_blocks'], $GLOBALS['CODE_CONTAINER_inputs'], $blocks);
$inputconnection = generateIOsSourceSetters($GLOBALS['CODE_CONTAINER_blocks'], $GLOBALS['CODE_CONTAINER_inputs'], $GLOBALS['CODE_CONTAINER_outputs'], $ios);

$GLOBALS['CODE_CONTAINER_connections'] = array_merge($blockconnection, $inputconnection);




//--------------MAIN ENDS HERE--------------

//--------------Project generation--------------
createProject($GLOBALS['COMPILE_ID']);

//-----START COMPILER-----
compileCode($GLOBALS['COMPILE_ID']);

//Zip compiled binary
zipBinary($GLOBALS['COMPILE_ID']);






//--------------DEDICATED FUNCTIONS--------------

//GENERATE EVERY INPUT/OUTPUT IO`S CONSTRUCTOR BY "compile.json" and "iometadata.json"
//Add header depencies to the "CODE_CONTAINER_includeHeaders" ARRAY
function generateIOConstructors($ios, $io_meta)
{
  $in = array();
  $out = array();
  foreach ($ios as $key => $value) {

    //get ids
    $io_pin = $value->pin;
    $io_id = $value->mode;

    //inlude header if have to
    $include = $io_meta->IOmodes->$io_id->include;
    if (!in_array($include, $GLOBALS['CODE_CONTAINER_includeHeaders'])) {
      //add the include source code (cpp) to GLOBAL ARRAY
      array_push($GLOBALS['CODE_CONTAINER_includeHeaders'], $include);
    }

    //create constructor (replace the unique_id)
    $constructor = $io_meta->IOmodes->$io_id->constructor;
    $constructor = str_replace("unique_id", $io_pin, $constructor);
    //take it to IN or OUT array
    if ($io_meta->IOmodes->$io_id->side == "IN")
      $in[$io_pin] = $constructor;
    else
      $out[$io_pin] = $constructor;
  }
  return array($in, $out);
}

//GENERATE EVERY BLOCK`S CONSTRUCTOR BY "compile.json" and "blockmetadata.json"
//Add header depencies to the "CODE_CONTAINER_includeHeaders" ARRAY
function generateBlockConstructors($blocks, $blocks_meta)
{
  $blockConstructors = array();
  foreach ($blocks as $key => $value) {

    //get ids
    $block_id = $value->classID;
    $unique_id = $value->uniqueID;

    //inlude header if have to
    $include = $blocks_meta->blocks->$block_id->include;
    if (!in_array($include, $GLOBALS['CODE_CONTAINER_includeHeaders'])) {
      //add it
      array_push($GLOBALS['CODE_CONTAINER_includeHeaders'], $include);
    }

    //create constructor (replace the unique_id)
    $constructor = $blocks_meta->blocks->$block_id->constructor;
    $constructor = str_replace("unique_id", $unique_id, $constructor);
    //insert it to block_container
    $blockConstructors[$unique_id] = $constructor;
  }
  return $blockConstructors;
}

//CREATE A FIX SIZE GLOBAL CONTAINER
function createGlobalContainer($constructors, $globalContainer_CPP, $globalContainerSize_CPP)
{
  $size = count($constructors);

  $container = str_replace("__size__", $size, $globalContainer_CPP);
  //Singleton check
  if (!in_array($container, $GLOBALS['CODE_CONTAINER_globals'])) {
    //add it
    array_push($GLOBALS['CODE_CONTAINER_globals'], $container);
    array_push($GLOBALS['CODE_CONTAINER_globals'], $globalContainerSize_CPP . $size . $GLOBALS['semicolon_CPP']);
  }
  return $size;
}

//CONTENT WRAPPER
//wrap the input content to any __content__ replacer in frame
function contentWrapper($frame, $content)
{
  return str_replace("__content__", $content, $frame);
}

//ADD CONSTRUCTED ELEMENTS TO A CONTAINER
//$container_SET_CPP: container template cpp source
//$constructors: constructor source cpp list
function generateLogicModulCreation($constructors, $container_SET_CPP)
{
  $constructed = array();
  $index = 0;
  //add it one by one start from 0. element
  foreach ($constructors as $unique_id => $constructor) {
    $line =  str_replace("__idx__", $index, $container_SET_CPP) . $constructor;
    //array_push($constructed, $line); //this for non-associative store
    $constructed[$unique_id] = $line; //this for associative store
    $index++;
  }
  return $constructed;
}

//GENERATE SOURCE CONNECTION SETTER CODES FOR BLOCKS
function generateBlocksSourceSetters($blockcontainer, $iocontainer, $connectionsObject)
{
  //connection adding cpp source
  $constructed = array();
  foreach ($connectionsObject as $key => $connection) {
    //
    $individual_block = array();
    $unique_id = $connection->uniqueID;
    //block input source list
    $input_array = $connection->inputs;
    foreach ($input_array as $input_id => $type_value) {
      //extract source type (FORMAT: type:unique_id.out_id, EXAMPLE: BLOCK:10.1)
      $type_value_splited = explode(":", $type_value);
      $type = $type_value_splited[0];
      $value = $type_value_splited[1];
      //NOTE: later optimise it
      //process type
      switch ($type) {
        case "BLOCK":
          //example: 1.2 -> 1:block`s unique id, 2:block`s output id 
          $value_splitted = explode(".", $value);
          $value_splitted[0] = $blockcontainer[$value_splitted[0]];
          $value_splitted[0] = explode("=", $value_splitted[0])[0];
          $source = $GLOBALS['getOutput_full_CPP'];
          $source = str_replace("__source__", $value_splitted[0], $source);
          $source = str_replace("__idx__", $value_splitted[1], $source);
          //source: *(globalBlockContainer[1]->getOutput(2))
          //echo $source;
          $value = $source;
          break;
        case "IO": {
            //example: 1.2 -> 1:io`s unique id, 2:io`s output id 
            $value_splitted = explode(".", $value);
            $value_splitted[0] = $iocontainer[$value_splitted[0]];
            $value_splitted[0] = explode("=", $value_splitted[0])[0];
            $source = $GLOBALS['getOutput_full_CPP'];
            $source = str_replace("__source__", $value_splitted[0], $source);
            $source = str_replace("__idx__", $value_splitted[1], $source);
            //source: *(globalBlockContainer[1]->getOutput(2))
            //echo $source;
            $value = $source;
            break;
          }
      }

      //get the reference to set in GLOBAL object
      $object_pointer = explode("=", $blockcontainer[$unique_id])[0];
      $setInput = $GLOBALS['setInput_CPP'];
      $setInput = str_replace("__idx__", $input_id, $setInput);
      $setInput = str_replace("__value__", $value, $setInput);
      array_push($individual_block, $object_pointer . $GLOBALS['arrayOperator_CPP'] . $setInput . $GLOBALS['semicolon_CPP']);
      //EXAMPLE OUTPUT LINE: globalBlockContainer[4]->setInput(1, *(globalBlockContainer[0]->getOutput(0)));
    }
    $constructed[$unique_id] = $individual_block;
  }

  return $constructed;
}

//GENERATE SOURCE CONNECTION SETTER CODES FOR IO BLOCKS
function generateIOsSourceSetters($blockcontainer, $iocontainer_in, $iocontainer_out, $connectionsObject)
{
  $constructed = array();
  //merge iocontainers
  $iocontainer = $iocontainer_in;
  foreach ($iocontainer_out as $key => $element) {
    $iocontainer[$key] = $element;
  }
  foreach ($connectionsObject as $key => $connection) {
    $individual_block = array();
    $unique_id = $connection->pin;
    $input_array = $connection->inputs;
    foreach ($input_array as $input_id => $type_value) {
      //extract source type
      $type_value_splited = explode(":", $type_value);
      $type = $type_value_splited[0];
      $value = $type_value_splited[1];
      //process type
      switch ($type) {
        case "BLOCK":
          //example: 1.2 -> 1:block`s unique id, 2:block`s output id 
          $value_splitted = explode(".", $value);
          $value_splitted[0] = $blockcontainer[$value_splitted[0]];
          $value_splitted[0] = explode("=", $value_splitted[0])[0];
          $source = $GLOBALS['getOutput_full_CPP'];
          $source = str_replace("__source__", $value_splitted[0], $source);
          $source = str_replace("__idx__", $value_splitted[1], $source);
          //source: *(globalBlockContainer[1]->getOutput(2))
          //echo $source;
          $value = $source;
          break;
        case "IO": {
            //example: 1.2 -> 1:io`s unique id, 2:io`s output id 
            $value_splitted = explode(".", $value);
            $value_splitted[0] = $iocontainer[$value_splitted[0]];
            $value_splitted[0] = explode("=", $value_splitted[0])[0];
            $source = $GLOBALS['getOutput_full_CPP'];
            $source = str_replace("__source__", $value_splitted[0], $source);
            $source = str_replace("__idx__", $value_splitted[1], $source);
            //source: *(globalBlockContainer[1]->getOutput(2))
            //echo $source;
            $value = $source;
            break;
          }
      }

      $object_pointer = explode("=", $iocontainer[$unique_id])[0];
      $setInput = $GLOBALS['setInput_CPP'];
      $setInput = str_replace("__idx__", $input_id, $setInput);
      $setInput = str_replace("__value__", $value, $setInput);
      array_push($individual_block, $object_pointer . $GLOBALS['arrayOperator_CPP'] . $setInput . $GLOBALS['semicolon_CPP']);
      //EXAMPLE OUTPUT LINE: globalOutputContainer[4]->setInput(0, *(globalBlockContainer[5]->getOutput(0)));
    }
    $constructed[$unique_id] = $individual_block;
  }

  return $constructed;
}

//SCHEDULE HELPER STRUCTURE CLASS
//EACH ELEMENT HAS A LAST CALCULATED QUEUE NUMBER
//itemname: the c++ source code fragment for Construct the BLOCK
//queueNumber: calculated queue number (it can increase but not decrease)
class ScheduleStruct
{
  public $queueNumber = 0;
  public $itemName;

  function __construct($itemName, $queueNumber)
  {
    $this->itemName = $itemName;
    $this->queueNumber = $queueNumber;
  }

  function setQueueNumber($queueNumber)
  {
    if ($queueNumber > $this->queueNumber) {
      $this->queueNumber = $queueNumber;
      return true;
    }
    return false;
  }
}

//RUN SCHEDULER
//IOS need for start output sources
//IT IS RUNS ONLY ON LOGIC BLOCKS
function runtimeSchedule($ios, $ios_meta, $blocks, $originalOrder)
{
  $schedulinglist = array();
  $queueNumber = 1;
  //iterate through hardware outputs
  foreach ($ios as $key => $ioblock) {
    //get ids
    $io_id = $ioblock->mode;
    //check OUT tag from HWIO meta table 
    if ($ios_meta->IOmodes->$io_id->side == "OUT") {
      //iterate through each source in normal case u have max one BLOCK connection
      foreach ($ioblock->inputs as $key => $source) {
        //extract source type
        $type_value_splited = explode(":", $source);
        if ($type_value_splited[0] == "BLOCK") {
          //get source block`s unique_id
          $source_block = explode(".", $type_value_splited[1])[0];
          //add new StructBlock object to schedulinglist
          $newEntry = new ScheduleStruct($originalOrder[$source_block], $queueNumber);
          $schedulinglist[$source_block] = $newEntry;

          //here $block_highlight have to point only the next DFS block from raw object (unique_id)
          $block_highlight = $source_block;
          scheduleDFS($schedulinglist, $blocks, $block_highlight, $originalOrder, $queueNumber + 1);
        }
      }
    }
  }
  //if $schedulinglist.size != $originalOrder.size => has blocks which non reachable throw outputs
  //iterate throug unused object and use all of them as DFS tree root (like a hardware output)
  foreach ($blocks as $key => $block) {
    if (!isset($schedulinglist[$key])) {
      //select unscheduled blocks`s unique_id as root in DFS
      $block_highlight = $key;
      //add this block as StructBlock object to schedulinglist
      $newEntry = new ScheduleStruct($originalOrder[$key], $queueNumber);
      $schedulinglist[$key] = $newEntry;
      scheduleDFS($schedulinglist, $blocks, $block_highlight, $originalOrder, $queueNumber + 1);
    }
  }
  //construct the scheduled_list from $schedulinglist by parse the key and name and order it by queueNumber(higher is first)
  $scheduled_list = createScheduling($schedulinglist);
  return $scheduled_list;
}

//RECURSION FUNCTION FOR DFS SEARCH TO BY CONNECTION TREE
//all found element close to a ScheduleStruct instance
//$schedulinglist: already found in tree (has to check first to update if has second link match)
//$blocks: conatins raw block object list for connections
//$originalOrder:original list to get the elements for DFS search output
function scheduleDFS(&$schedulinglist, $blocks, $block_highlight, $originalOrder, $queueNumber)
{
  foreach ($blocks->$block_highlight->inputs as $key => $source) {
    //extract source type
    $type_value_splited = explode(":", $source);
    //search throug only BLOCK connections
    if ($type_value_splited[0] == "BLOCK") {
      //get source block`s unique_id
      $source_block = explode(".", $type_value_splited[1])[0];
      //check self-loop -> continue
      if ($source_block == $block_highlight) {
        continue;
      }
      //if found element already in the $schedulinglist update the queue
      if (in_array($source_block, $schedulinglist)) {
        if (!$schedulinglist[$source_block]->setQueueNumber()) {
          continue;
        }
      } else {
        $newEntry = new ScheduleStruct($originalOrder[$source_block], $queueNumber);
        $schedulinglist[$source_block] = $newEntry;
      }
      $block_highlight = $source_block;
      //here $block_highlight have to point only the next DFS block from raw object (unique_id)
      scheduleDFS($schedulinglist, $blocks, $block_highlight, $originalOrder, $queueNumber + 1);
    }
  }
}

//$schedulinglist is a 1D ScheduleStruct array 
function createScheduling($schedulinglist)
{
  //$organisedObjectsByQueue is 2D array[queueNumber][queueElements]
  $organisedObjectsByQueue = array();
  foreach ($schedulinglist as $key => $elementStruct) {
    //check queueNumber existence in 2D array head
    if (!array_key_exists($elementStruct->queueNumber, $organisedObjectsByQueue)) {
      //create new entry row
      $organisedObjectsByQueue[$elementStruct->queueNumber] = array();
    }
    //add the element under the proper queueNumber list
    $organisedObjectsByQueue[$elementStruct->queueNumber][$key] = $elementStruct->itemName;
  }
  //sort the 2D array by queue high to low
  krsort($organisedObjectsByQueue);
  //transform 2D array to 1D and put itemnames in it (cpp source constructors)
  $scheduledlist = array();
  foreach ($organisedObjectsByQueue as $queue_number => $queue_element) {
    foreach ($queue_element as $key => $block_constructor) {
      $scheduledlist[$key] = $block_constructor;
    }
  }
  return $scheduledlist;
}

function createProject($compileID)
{
  mkdir("arduino_cli/projects/" . $compileID);
  //---Write out to file---

  $outputINOfile = fopen("arduino_cli/projects/" . $GLOBALS['COMPILE_ID'] . "/" . $GLOBALS['COMPILE_ID'] . ".ino", "w");

  foreach ($GLOBALS['CODE_CONTAINER_includeHeaders'] as $CODE_CONTAINER_includeHeader) {
    //echo $CODE_CONTAINER_includeHeader . "<br>";
    fwrite($outputINOfile, $CODE_CONTAINER_includeHeader . "\n");
  }
  foreach ($GLOBALS['CODE_CONTAINER_globals'] as $CODE_CONTAINER_global) {
    //echo $CODE_CONTAINER_global . "<br>";
    fwrite($outputINOfile, $CODE_CONTAINER_global . "\n");
  }
  foreach ($GLOBALS['CODE_CONTAINER_inputs'] as $CODE_CONTAINER_input) {
    $GLOBALS['CODE_CONTAINER_setup'] = $CODE_CONTAINER_input . "\n";
  }
  foreach ($GLOBALS['CODE_CONTAINER_outputs'] as $CODE_CONTAINER_output) {
    $GLOBALS['CODE_CONTAINER_setup'] = $GLOBALS['CODE_CONTAINER_setup'] . $CODE_CONTAINER_output . "\n";
  }
  foreach ($GLOBALS['CODE_CONTAINER_blocks'] as $key => $CODE_CONTAINER_block) {
    $GLOBALS['CODE_CONTAINER_setup'] = $GLOBALS['CODE_CONTAINER_setup'] . $CODE_CONTAINER_block . "\n";
  }

  foreach ($GLOBALS['CODE_CONTAINER_connections'] as $key => $CODE_CONTAINER_connection) {
    foreach ($CODE_CONTAINER_connection as $key => $element) {
      $GLOBALS['CODE_CONTAINER_setup'] = $GLOBALS['CODE_CONTAINER_setup'] . $element . "\n";
    }
  }


  //wrap setup and write out
  fwrite($outputINOfile, contentWrapper($GLOBALS['arduinoSETUP_CPP'], $GLOBALS['CODE_CONTAINER_setup']));
  //wrap loop and write out
  fwrite($outputINOfile, contentWrapper($GLOBALS['arduinoLOOP_CPP'], $GLOBALS['runModules_CPP']));

  fclose($outputINOfile);



  //DEBUG ECHO TO WEB
  $myfile = fopen("arduino_cli/projects/" . $GLOBALS['COMPILE_ID'] . "/" . $GLOBALS['COMPILE_ID'] . ".ino", "r") or die("Unable to open file!");
  while (!feof($myfile)) {
    echo fgets($myfile) . "<br>";
  }
  fclose($myfile);
}

function compileCode($compileID)
{
  mkdir("arduino_cli/builds/" . $compileID);
  $cmd = 'arduino_cli\arduino-cli --config-file arduino_cli/libraries/' . $GLOBALS['boardId'] . '/' . $GLOBALS['libraryVer'] . '/config.yaml compile --fqbn ' . $GLOBALS['boardFQBN'] . ' arduino_cli/projects/' . $compileID . ' --build-path arduino_cli/builds/' . $compileID . ' -e';
  //$cmd = 'arduino_cli\arduino-cli --config-file arduino_cli/libraries/' . $GLOBALS['boardId'] . '/' . $GLOBALS['libraryVer'] . '/config.yaml compile --fqbn ' . $GLOBALS['boardFQBN'] . ' arduino_cli/projects/' . $compileID . ' -e';
  shell_exec('powershell.exe -command "' . $cmd . '"  2>&1');
}

function zipBinary($compileID)
{

  $buildPath = "arduino_cli/projects/" . $compileID . "/build/" . $GLOBALS['platform'];
  $exportpath = "compiled/" . $GLOBALS['projectName'] . "-" . $compileID . ".zip";

  $rootPath = realpath($buildPath);

  // Initialize archive object
  $zip = new ZipArchive();
  $zip->open($exportpath, ZipArchive::CREATE | ZipArchive::OVERWRITE);

  // Create recursive directory iterator
  /** @var SplFileInfo[] $files */
  $files = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($rootPath),
    RecursiveIteratorIterator::LEAVES_ONLY
  );

  foreach ($files as $name => $file) {
    // Skip directories (they would be added automatically)
    if (!$file->isDir()) {
      // Get real and relative path for current file
      $filePath = $file->getRealPath();
      $relativePath = substr($filePath, strlen($rootPath) + 1);

      // Add current file to archive
      $zip->addFile($filePath, $relativePath);
    }
  }

  // Zip archive will be created only after closing object
  $zip->close();
}
