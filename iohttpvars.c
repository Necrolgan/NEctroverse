char *iohttpVarsRaw;


int iohttpVarsSize;



char *iohttpVarsFind( char *id )
{
  int a;
  char *src;
  if( !( iohttpVarsRaw ) )
    return 0;
  src = iohttpVarsRaw;
  for( ; ; )
  {
    for( a = 0 ; ; a++ )
    {
      if( !( id[a] ) && ( src[a] == '=' ) )
        return ( src + a + 1 );
      if( src[a] != id[a] )
        break;
      if( !( src[a] ) )
        return 0;
    }
    for( ; ; src++ )
    {
      if( src[0] == '&' )
      {
        src++;
        break;
      }
      if( !( src[0] ) )
        return 0;
    }
  }
  return 0;
}


char *iohtmlVarsFind( ReplyDataPtr cnt, char *id ) {
	int a;
	char *value;	
	
for( a = 0; a < (cnt->session)->posts; a++ ) {
	if( strcmp( id, (cnt->session)->key[a] ) == 0 )
		return (cnt->session)->value[a];
}

value = (char *)MHD_lookup_connection_value(cnt->connection, MHD_GET_ARGUMENT_KIND, id);
if( ( value ) && strlen(value) ) {
	return value;
}

return 0;
}

char *iohtmlHeaderFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_HEADER_KIND, id);
}

char *iohtmlCookieFind( ReplyDataPtr cnt, char *id ) {

return (char *)MHD_lookup_connection_value(cnt->connection, MHD_COOKIE_KIND, id);
}

int iohtmlCookieAdd( ReplyDataPtr cnt, char *name, char *value, ... ) {
	char cstr[256];
	char text[256];
	va_list ap;

va_start( ap, value );
vsnprintf( text, 256, value, ap );
va_end( ap );

snprintf(cstr, sizeof (cstr), "%s=%s", name, text);

strncpy((cnt->cookies).value[(cnt->cookies).num], cstr, sizeof(cstr) );
(cnt->cookies).num = (cnt->cookies).num + 1;
	

return (cnt->cookies).num;
}


void iohttpVarsCut()
{
  char *src;
  if( !( iohttpVarsRaw ) )
    return;
  src = iohttpVarsRaw;
  for( ; ; src++ )
  {
    if( src[0] == '&' )
      src[0] = 0;
    else if( src[0] == 0 )
      return;
  }
  return;
}



int iohttpVarsInit( svConnectionPtr cnt )
{
  iohttpDataPtr iohttp = cnt->iodata;
  if( iohttp->method == 3 )
    iohttpVarsRaw = iohttp->content;
  else
    iohttpVarsRaw = iohttp->query_string;
  return 1;
}

//FIXME
int iohttpVarsMapcoords( svConnectionPtr cnt, int *coords )
{
  iohttpDataPtr iohttp = cnt->iodata;
  char *string;
  if( iohttp->method == 3 )
    string = iohttp->content;
  else
    string = iohttp->query_string;
  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[0] ) != 1 )
    return 0;
  string = ioCompareFindWords( string, "," );
  if( !( string ) )
    return 0;
  if( sscanf( string, "%d", &coords[1] ) != 1 )
    return 0;
  return 1;
}


/*
  iohttpVarsInit();

  params[0] = iohttpVarsFind( "f1" );
  params[1] = iohttpVarsFind( "f2" );
  iohttpVarsCut();

  if( params[0] )
    printf( "<font color=\"#FFFFFF\">f1 : %s</font><br>", params[0] );
  if( params[1] )
    printf( "<font color=\"#FFFFFF\">f2 : %s</font><br>", params[1] );
*/


char *iohttpVarsUpload( svConnectionPtr cnt, char **filename, int *filesize )
{
  int size;
  iohttpDataPtr iohttp = cnt->iodata;
  char *boundary, *content, *end, *fname;
  if( !( iohttp->content_type ) || !( iohttp->content ) )
    return 0;
  boundary = ioCompareFindWords( iohttp->content_type, "boundary=" );
  if( !( boundary ) )
    return 0;
  content = ioCompareFindWords( iohttp->content, "\r\n\r\n" );
  if( !( content ) )
    content = ioCompareFindWords( iohttp->content, "\n\n" );
  if( !( content ) )
    return 0;
  if( filename )
  {
    fname = ioCompareFindWords( iohttp->content, "filename=\"" );
    *filename = fname;
  }
  end = ioCompareFindBinwords( iohttp->content, boundary, iohttp->content_length );
  if( !( end ) )
    return 0;
  end = ioCompareFindBinwords( end, boundary, iohttp->content_length - ( end - iohttp->content ) );
  if( !( end ) )
    return 0;
  end -= strlen( boundary ) + 4;
  size = end - content;
  content[size] = 0;
  *filesize = size;
  return content;
}





