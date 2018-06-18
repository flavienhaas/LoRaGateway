void transform(char stationName[], uint16_t dataArray[])
{
  // 9 characters on 5 bits
  // 1 number on 3 bits
  // Frame : xxxxx|xxx.xx|xxxxx|x.xxxx|xxxx.x|xxxxx|xx.xxx|xxxxx|.xxxxx|xxx
  
  dataArray[0] = (dataArray[0] << 5) | convertCharacterToNumber(stationName[0]);
  dataArray[0] = (dataArray[0] << 5) | convertCharacterToNumber(stationName[1]);
  dataArray[0] = (dataArray[0] << 5) | convertCharacterToNumber(stationName[2]);
  dataArray[0] = (dataArray[0] << 1) | (convertCharacterToNumber(stationName[3]) >> 4);

  dataArray[1] = (dataArray[1] << 5) | convertCharacterToNumber(stationName[3]);
  dataArray[1] = (dataArray[1] << 5) | convertCharacterToNumber(stationName[4]);
  dataArray[1] = (dataArray[1] << 5) | convertCharacterToNumber(stationName[5]);
  dataArray[1] = (dataArray[1] << 2) | (convertCharacterToNumber(stationName[6]) >> 3);

  dataArray[2] = (dataArray[2] << 5) | convertCharacterToNumber(stationName[6]);
  dataArray[2] = (dataArray[2] << 5) | convertCharacterToNumber(stationName[7]);
  dataArray[2] = (dataArray[2] << 5) | convertCharacterToNumber(stationName[8]);
  dataArray[2] = (dataArray[2] << 3) | convertCharacterNumberToNumber(stationName[9]);
}

uint8_t convertCharacterNumberToNumber(char myCharacter)
{
  switch(myCharacter)
  {
    case '0':
    {
      return(0);
      break;
    }

    case '1':
    {
      return(1);
      break;
    }

    case '2':
    {
      return(2);
      break;
    }

    case '3':
    {
      return(3);
      break;
    }

    case '4':
    {
      return(4);
      break;
    }

    case '5':
    {
      return(5);
      break;
    }

    case '6':
    {
      return(6);
      break;
    }

    case '7':
    {
      return(7);
      break;
    }
  }
}

uint8_t convertCharacterToNumber(char myCharacter)
{
  switch(myCharacter)
  {
    case 'A':
    {
      return(0);
      break;
    }

    case 'B':
    {
      return(1);
      break;
    }

    case 'C':
    {
      return(2);
      break;
    }

    case 'D':
    {
      return(3);
      break;
    }

    case 'E':
    {
      return(4);
      break;
    }

    case 'F':
    {
      return(5);
      break;
    }

    case 'G':
    {
      return(6);
      break;
    }

    case 'H':
    {
      return(7);
      break;
    }

    case 'I':
    {
      return(8);
      break;
    }

    case 'J':
    {
      return(9);
      break;
    }

    case 'K':
    {
      return(10);
      break;
    }

    case 'L':
    {
      return(11);
      break;
    }

    case 'M':
    {
      return(12);
      break;
    }

    case 'N':
    {
      return(13);
      break;
    }

    case 'O':
    {
      return(14);
      break;
    }

    case 'P':
    {
      return(15);
      break;
    }

    case 'Q':
    {
      return(16);
      break;
    }

    case 'R':
    {
      return(17);
      break;
    }

    case 'S':
    {
      return(18);
      break;
    }

    case 'T':
    {
      return(19);
      break;
    }

    case 'U':
    {
      return(20);
      break;
    }

    case 'V':
    {
      return(21);
      break;
    }

    case 'W':
    {
      return(22);
      break;
    }

    case 'X':
    {
      return(23);
      break;
    }

    case 'Y':
    {
      return(24);
      break;
    }
    
    case 'Z':
    {
      return(25);
      break;
    }

    case '-':
    {
      return(26);
      break;
    }

    case '.':
    {
      return(27);
      break;
    }

    case '/':
    {
      return(28);
      break;
    }

    case '\rr':
    {
      return(29);
      break;
    }

    case '(':
    {
      return(30);
      break;
    }

    case ')':
    {
      return(31);
      break;
    }
  }
}
