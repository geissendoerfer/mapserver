/**********************************************************************
 * $Id$
 *
 * Name:     mapcontext.c
 * Project:  MapServer
 * Language: C
 * Purpose:  OGC Web Map Context implementation
 * Author:   Julien-Samuel Lacroix, DM Solutions Group (lacroix@dmsolutions.ca)
 *
 **********************************************************************
 * Copyright (c) 2002-2003, Julien-Samuel Lacroix, DM Solutions Group Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 * $Log$
 * Revision 1.57  2004/08/03 23:26:24  dan
 * Cleanup OWS version tests in the code, mapwms.c (bug 799)
 *
 * Revision 1.56  2004/08/03 22:12:34  dan
 * Cleanup OWS version tests in the code, started with mapcontext.c (bug 799)
 *
 * Revision 1.55  2004/06/22 20:55:20  sean
 * Towards resolving issue 737 changed hashTableObj to a structure which contains a hashObj **items.  Changed all hash table access functions to operate on the target table by reference.  msFreeHashTable should not be used on the hashTableObj type members of mapserver structures, use msFreeHashItems instead.
 *
 * Revision 1.54  2004/04/16 19:12:31  assefa
 * Correct bug on windows when opening xml file (open it in binary mode).
 *
 * Revision 1.53  2004/04/14 05:14:54  dan
 * Added ability to pass a default value to msOWSPrintMetadataList() (bug 616)
 *
 * Revision 1.52  2004/04/14 04:54:30  dan
 * Created msOWSLookupMetadata() and added namespaces lookup in all
 * msOWSPrint*Metadata() functions. Also pass namespaces=NULL everywhere
 * that calls those functions for now to avoid breaking something just
 * before the release. (bug 615, 568)
 *
 * Revision 1.51  2003/12/23 20:40:38  julien
 * Replace legendurl, logourl, descriptionurl, dataurl and metadataurl metadata
 * by four new metadata by metadata replaced. The new metadata are called
 * legendurl_width, legendurl_height, legendurl_format, legendurl_href,
 * logourl_width, etc...
 * Old dependancy to the metadata with four value, space separated, are kept.
 *
 * Revision 1.50  2003/12/22 17:00:21  julien
 * Implement DataURL, MetadataURL and DescriptionURL (Bug 523)
 *
 * Revision 1.49  2003/07/31 16:10:34  dan
 * Enable map context stuff only if USE_OGR is set (cpl_minixml dependency)
 *
 * Revision 1.48  2003/07/15 15:22:23  assefa
 * Modify schema location url.
 *
 * Revision 1.47  2003/07/15 14:07:07  assefa
 * replace View_context by ViewContext for 1.0. support.
 *
 * Revision 1.46  2003/07/11 15:43:20  dan
 * Try to pick a supported format when current format is not supported
 *
 * Revision 1.45  2003/06/26 12:43:14  assefa
 * typo : replace printf by fprintf.
 *
 * Revision 1.44  2003/06/26 02:49:47  assefa
 * Add support for version 1.0.0
 *
 * Revision 1.43  2003/04/09 07:13:49  dan
 * Added GetContext (custom) request in WMS interface.
 * Added missing gml: namespace in 0.1.7 context output.
 *
 * Revision 1.42  2003/03/07 21:22:50  julien
 * Fix a typo in ContactFacsimileTelephone
 *
 * Revision 1.41  2003/02/21 19:19:49  julien
 * Do not put 'init=' before the srs to proj when it begin by 'AUTO:'
 *
 * Revision 1.40  2003/02/04 14:33:18  julien
 * Fix the closing tag of View_Context
 *
 * Revision 1.39  2003/01/30 22:46:32  julien
 * mv context_* metadata to wms_context_*
 *
 * Revision 1.38  2003/01/30 22:43:29  julien
 * Remove logourl heritage and customize encoding
 *
 * Revision 1.37  2003/01/30 21:17:03  julien
 * Implement the version 0.1.7
 *
 * Revision 1.36  2002/12/20 20:21:13  julien
 * wms_style__title is now set correctly
 *
 * Revision 1.35  2002/12/19 19:26:11  julien
 * Don't set the projection for each layer
 *
 * Revision 1.34  2002/12/11 18:10:39  julien
 * Remove possible WARNING inside other tags
 *
 * Revision 1.33  2002/11/29 19:28:17  julien
 * Replace ' ' by ',' in stylelist and formatlist srs
 *
 * Revision 1.32  2002/11/27 21:00:44  julien
 * Fatal error if server version is missing in load function
 *
 * Revision 1.31  2002/11/27 20:14:19  julien
 * Use the map srs if no layer srs is specify
 *
 * Revision 1.30  2002/11/26 01:37:55  dan
 * Fixed compile warnings
 *
 * Revision 1.29  2002/11/25 21:48:07  dan
 * Set map units after setting new projections in msLoadMapContext()
 *
 * Revision 1.28  2002/11/25 14:48:10  julien
 * One SRS tag with multiple SRS elements space separated
 *
 * Revision 1.27  2002/11/22 21:50:33  julien
 * Fix SRS and LegendURL for 0.1.2
 *
 * Revision 1.26  2002/11/22 17:42:52  julien
 * Support DataURL and LogoURL for 0.1.2 version
 *
 * Revision 1.25  2002/11/22 15:12:51  julien
 * Fix the seg fault temporaly fixed on 2002/11/21 17:07:34
 *
 * Revision 1.24  2002/11/21 20:39:40  julien
 * Support the wms_srs metadata and support multiple SRS tag
 *
 * Revision 1.23  2002/11/21 17:07:34  julien
 * temporaly fix a seg. fault with pszSLD2
 *
 * Revision 1.22  2002/11/21 15:54:46  julien
 * Valid empty Format and style, some chage to 0.1.2 version
 *
 * Revision 1.21  2002/11/20 23:57:55  julien
 * Remove duplicated code of style and format and create a wms_name
 *
 * Revision 1.20  2002/11/20 22:17:09  julien
 * Replace fatal error by msDebug
 *
 * Revision 1.19  2002/11/20 21:25:35  dan
 * Duh! Forgot to set the proper path for the contexts/0.1.4/context.xsd
 *
 * Revision 1.18  2002/11/20 21:22:32  dan
 * Added msOWSGetSchemasLocation() for use by both WFS and WMS Map Context
 *
 * Revision 1.17  2002/11/20 19:08:55  julien
 * Support onlineResource of version 0.1.2
 *
 * Revision 1.16  2002/11/20 17:17:21  julien
 * Support version 0.1.2 of MapContext
 * Remove warning from tags
 * Encode and decode all url
 *
 * Revision 1.15  2002/11/15 18:53:10  assefa
 * Correct test on fread cuasing problems on Windows.
 *
 * Revision 1.14  2002/11/14 18:46:14  julien
 * Include the ifdef to compile without the USE_WMS_LYR tag
 *
 * Revision 1.13  2002/11/13 16:54:23  julien
 * Change the search of the header to be flexible.
 *
 * Revision 1.12  2002/11/07 21:50:19  julien
 * Set the layer type to RASTER
 *
 * Revision 1.11  2002/11/07 21:16:45  julien
 * Fix warning in ContactInfo
 *
 * Revision 1.10  2002/11/07 15:53:14  julien
 * Put duplicated code in a single function
 * Fix many typo error
 *
 * Revision 1.9  2002/11/05 21:56:13  julien
 * Remove fatal write mistake in msSaveMapContext
 *
 * Revision 1.8  2002/11/04 20:39:17  julien
 * Change a validation to prevent a crash
 *
 * Revision 1.6  2002/11/04 20:16:09  julien
 * Change a validation to prevent a crash
 *
 * Revision 1.5  2002/10/31 21:25:55  julien
 * transform EPSG:*** to init=epsg:***
 *
 * Revision 1.4  2002/10/31 20:00:07  julien
 * transform EPSG:*** to init=epsg:***
 *
 * Revision 1.3  2002/10/31 18:57:35  sacha
 * Fill layerorder and layer.index in msLoadMapContext
 *
 * Revision 1.2  2002/10/28 22:31:09  dan
 * Added map arg to initLayer() call
 *
 * Revision 1.1  2002/10/28 20:31:20  dan
 * New support for WMS Map Context (from Julien)
 *
 * Revision 1.1  2002/10/22 20:03:57  julien
 * Add the mapcontext support
 *
 **********************************************************************/

#include "map.h"

#if defined(USE_WMS_LYR) && defined(USE_OGR)

/* There is a dependency to GDAL/OGR for the GML driver and MiniXML parser */
#include "cpl_minixml.h"

#endif

/* msGetMapContextFileText()
**
** Read a file and return is content
**
** Take the filename in argument
** Return value must be freed by caller
*/
char * msGetMapContextFileText(char *filename)
{
  char *pszBuffer;
  FILE *stream;
  int	 nLength;
 
  // open file
  if(filename != NULL && strlen(filename) > 0) {
      stream = fopen(filename, "rb");
      if(!stream) {
          msSetError(MS_IOERR, "(%s)", "msGetMapContextFileText()", filename);
          return NULL;
      }
  }
  else
  {
      msSetError(MS_IOERR, "(%s)", "msGetMapContextFileText()", filename);
      return NULL;
  }

  fseek( stream, 0, SEEK_END );
  nLength = ftell( stream );
  fseek( stream, 0, SEEK_SET );

  pszBuffer = (char *) malloc(nLength+1);
  if( pszBuffer == NULL )
  {
      msSetError(MS_MEMERR, "(%s)", "msGetMapContextFileText()", filename);
      fclose( stream );
      return NULL;
  }
  
  if(fread( pszBuffer, nLength, 1, stream ) == 0 &&  !feof(stream))
  {
      free( pszBuffer );
      fclose( stream );
      msSetError(MS_IOERR, "(%s)", "msGetMapContextFileText()", filename);
      return NULL;
  }
  pszBuffer[nLength] = '\0';

  fclose( stream );

  return pszBuffer;
}


#if defined(USE_WMS_LYR) && defined(USE_OGR)

/*
**msGetMapContextXMLHashValue()
**
**Get the xml value and put it in the hash table
**
*/
int msGetMapContextXMLHashValue( CPLXMLNode *psRoot, const char *pszXMLPath, 
                                 hashTableObj *metadata, char *pszMetadata )
{
  char *pszValue;

  pszValue = (char*)CPLGetXMLValue( psRoot, pszXMLPath, NULL);
  if(pszValue != NULL)
  {
      if( metadata != NULL )
      {
          msInsertHashTable(metadata, pszMetadata, pszValue );
      }
      else
      {
          return MS_FAILURE;
      }
  }
  else
  {
      return MS_FAILURE;
  }

  return MS_SUCCESS;
}

/*
**msGetMapContextXMLHashValue()
**
**Get the xml value and put it in the hash table
**
*/
int msGetMapContextXMLHashValueDecode( CPLXMLNode *psRoot, 
                                       const char *pszXMLPath, 
                                       hashTableObj *metadata, 
                                       char *pszMetadata )
{
  char *pszValue;

  pszValue = (char*)CPLGetXMLValue( psRoot, pszXMLPath, NULL);
  if(pszValue != NULL)
  {
      if( metadata != NULL )
      {
          msDecodeHTMLEntities(pszValue);
          msInsertHashTable(metadata, pszMetadata, pszValue );
      }
      else
      {
          return MS_FAILURE;
      }
  }
  else
  {
      return MS_FAILURE;
  }

  return MS_SUCCESS;
}


/*
**msGetMapContextXMLStringValue()
**
**Get the xml value and put it in the string field
**
*/
int msGetMapContextXMLStringValue( CPLXMLNode *psRoot, char *pszXMLPath, 
                                   char **pszField)
{
  char *pszValue;

  pszValue = (char*)CPLGetXMLValue( psRoot, pszXMLPath, NULL);
  if(pszValue != NULL)
  {
      if( pszField != NULL )
      {
           *pszField = strdup(pszValue);
      }
      else
      {
          return MS_FAILURE;
      }
  }
  else
  {
      return MS_FAILURE;
  }

  return MS_SUCCESS;
}


/*
**msGetMapContextXMLStringValue()
**
**Get the xml value and put it in the string field
**
*/
int msGetMapContextXMLStringValueDecode( CPLXMLNode *psRoot, char *pszXMLPath, 
                                         char **pszField)
{
  char *pszValue;

  pszValue = (char*)CPLGetXMLValue( psRoot, pszXMLPath, NULL);
  if(pszValue != NULL)
  {
      if( pszField != NULL )
      {
          msDecodeHTMLEntities(pszValue);
          *pszField = strdup(pszValue);
      }
      else
      {
          return MS_FAILURE;
      }
  }
  else
  {
      return MS_FAILURE;
  }

  return MS_SUCCESS;
}


/*
**msGetMapContextXMLFloatValue()
**
**Get the xml value and put it in the string field
**
*/
int msGetMapContextXMLFloatValue( CPLXMLNode *psRoot, char *pszXMLPath, 
                                        double *pszField)
{
  char *pszValue;

  pszValue = (char*)CPLGetXMLValue( psRoot, pszXMLPath, NULL);
  if(pszValue != NULL)
  {
      if( pszField != NULL )
      {
          *pszField = atof(pszValue);
      }
      else
      {
          return MS_FAILURE;
      }
  }
  else
  {
      return MS_FAILURE;
  }

  return MS_SUCCESS;
}

#endif

/* msLoadMapContext()
**
** Get a mapfile from a OGC Web Map Context format
**
** Take a map object and a file in arguments
*/
int msLoadMapContext(mapObj *map, char *filename)
{
#if defined(USE_WMS_LYR) && defined(USE_OGR)
  char *pszWholeText, *pszMapProj=NULL, *pszProj=NULL;
  char *pszValue, *pszValue1, *pszValue2;
  char *pszHash, *pszStyle=NULL, *pszStyleName, *pszName=NULL;
  CPLXMLNode *psRoot, *psContactInfo, *psMapContext, *psLayer, *psLayerList;
  CPLXMLNode *psFormatList, *psFormat, *psStyleList, *psStyle, *psChild, *psKeyword, 
    *psKeywordList;
  CPLXMLNode *psLegendURL, *psSRS;
  char szPath[MS_MAXPATHLEN];
  int nStyle, nVersion=-1;
  layerObj *layer;
  char szVersionBuf[OWS_VERSION_MAXLEN];

  //
  // Load the raw XML file
  //
  
  pszWholeText = msGetMapContextFileText(
      msBuildPath(szPath, map->mappath, filename));
  if(pszWholeText == NULL)
  {
      msSetError( MS_MAPCONTEXTERR, "Unable to read %s", 
                  "msLoadMapContext()", filename );
      return MS_FAILURE;
 }

  if( ( strstr( pszWholeText, "<WMS_Viewer_Context" ) == NULL ) &&
      ( strstr( pszWholeText, "<View_Context" ) == NULL ) &&
      ( strstr( pszWholeText, "<ViewContext" ) == NULL ) )
    
  {
      free( pszWholeText );
      msSetError( MS_MAPCONTEXTERR, "Not a Map Context file (%s)", 
                  "msLoadMapContext()", filename );
      return MS_FAILURE;
  }

  //
  // Convert to XML parse tree.
  //
  psRoot = CPLParseXMLString( pszWholeText );
  free( pszWholeText );

  // We assume parser will report errors via CPL.
  if( psRoot == NULL || psRoot->psNext == NULL )
  {
      msSetError( MS_MAPCONTEXTERR, "Invalid XML file (%s)", 
                  "msLoadMapContext()", filename );
      return MS_FAILURE;
  }

  // Valid the MapContext file and get the root of the document
  psChild = psRoot;
  psMapContext = NULL;
  while( psChild != NULL )
  {
      if( psChild->eType == CXT_Element && 
          (EQUAL(psChild->pszValue,"WMS_Viewer_Context") ||
           EQUAL(psChild->pszValue,"View_Context") ||
           EQUAL(psChild->pszValue,"ViewContext")) )
      {
          psMapContext = psChild;
          break;
      }
      else
      {
          psChild = psChild->psNext;
      }
  }

  if( psMapContext == NULL )
  {
      CPLDestroyXMLNode(psRoot);
      msSetError( MS_MAPCONTEXTERR, "Invalid Map Context File (%s)", 
                  "msLoadMapContext()", filename );
      return MS_FAILURE;
  }

  // Fetch document version number
  pszValue = (char*)CPLGetXMLValue(psMapContext, 
                                   "version", NULL);
  if( !pszValue )
  {
      msDebug( "msLoadMapContext(): Mandatory data version missing in %s, assuming 0.1.4.",
               filename );
      pszValue = "0.1.4";
  }

  nVersion = msOWSParseVersionString(pszValue);

  // Make sure this is a supported version
  switch (nVersion)
  {
    case OWS_0_1_2:
    case OWS_0_1_4:
    case OWS_0_1_7:
    case OWS_1_0_0:
      // All is good, this is a supported version.
      break;
    default:
      // Not a supported version
      msSetError(MS_MAPCONTEXTERR, 
                 "This version of Map Context is not supported (%s).",
                 "msLoadMapContext()", pszValue);
      CPLDestroyXMLNode(psRoot);
      return MS_FAILURE;
  }

  // Load the metadata of the WEB obj
  //if(map->web.metadata == NULL)
  //    &(map->web.metadata) =  msCreateHashTable();

  // Reformat and save Version in metadata
  msInsertHashTable( &(map->web.metadata), "wms_context_version",
                     msOWSGetVersionString(nVersion, szVersionBuf));

  if( nVersion >= OWS_0_1_7 && nVersion < OWS_1_0_0)
  {
      if( msGetMapContextXMLHashValue(psMapContext, "fid", 
                           &(map->web.metadata), "wms_context_fid") == MS_FAILURE )
      {
          msDebug("Mandatory data fid missing in %s.", filename);
      }
  }

  // Projection
  pszValue = (char*)CPLGetXMLValue(psMapContext, 
                                   "General.BoundingBox.SRS", NULL);
  if(pszValue != NULL)
  {
      if(strncasecmp(pszValue, "AUTO:", 5) == 0)
      {
          pszProj = strdup(pszValue);
      }
      else
      {
          pszProj = (char*) malloc(sizeof(char)*(strlen(pszValue)+10));
          sprintf(pszProj, "init=epsg:%s", pszValue+5);
      }
      pszMapProj = strdup(pszValue);

      msInitProjection(&map->projection);
      map->projection.args[map->projection.numargs] = strdup(pszProj);
      map->projection.numargs++;
      msProcessProjection(&map->projection);

      if( (map->units = GetMapserverUnitUsingProj(&(map->projection))) == -1)
      {
          free(pszProj);
          msSetError( MS_MAPCONTEXTERR, 
                      "Unable to set units for projection '%s'",
                      "msLoadMapContext()", pszProj );
          CPLDestroyXMLNode(psRoot);
          return MS_FAILURE;
      }
      free(pszProj);
  }
  else
  {
      msDebug("Mandatory data General.BoundingBox.SRS missing in %s.",
              filename);
  }

  // Extent
  if( msGetMapContextXMLFloatValue(psMapContext, "General.BoundingBox.minx",
                                   &(map->extent.minx)) == MS_FAILURE)
  {
      msDebug("Mandatory data General.BoundingBox.minx missing in %s.",
              filename);
  }
  if( msGetMapContextXMLFloatValue(psMapContext, "General.BoundingBox.miny",
                               &(map->extent.miny)) == MS_FAILURE)
  {
      msDebug("Mandatory data General.BoundingBox.miny missing in %s.",
              filename);
  }
  if( msGetMapContextXMLFloatValue(psMapContext, "General.BoundingBox.maxx",
                               &(map->extent.maxx)) == MS_FAILURE)
  {
      msDebug("Mandatory data General.BoundingBox.maxx missing in %s.",
              filename);
  }
  if( msGetMapContextXMLFloatValue(psMapContext, "General.BoundingBox.maxy",
                               &(map->extent.maxy)) == MS_FAILURE)
  {
      msDebug("Mandatory data General.BoundingBox.maxy missing in %s.",
              filename);
  }

  // Title
  if( msGetMapContextXMLHashValue(psMapContext, "General.Title", 
                              &(map->web.metadata), "wms_title") == MS_FAILURE)
  {
      if ( nVersion >= OWS_1_0_0 )
         msDebug("Mandatory data General.Title missing in %s.", filename);
      else
      {
          if( msGetMapContextXMLHashValue(psMapContext, "General.gml:name", 
                             &(map->web.metadata), "wms_title") == MS_FAILURE )
          {
              if( nVersion < OWS_0_1_7 )
                msDebug("Mandatory data General.Title missing in %s.", filename);
              else
                msDebug("Mandatory data General.gml:name missing in %s.", 
                        filename);
          }
      }
  }

  // Name
  if( nVersion >= OWS_1_0_0 )
  {
      pszValue = (char*)CPLGetXMLValue(psMapContext, 
                                       "id", NULL);
      if (pszValue)
        map->name = strdup(pszValue);
  }
  else
  {
      if(msGetMapContextXMLStringValue(psMapContext, "General.Name", 
                                       &(map->name)) == MS_FAILURE)
      {
          msGetMapContextXMLStringValue(psMapContext, "General.gml:name", 
                                        &(map->name));
      }
  }
  // Keyword
  if( nVersion >= OWS_1_0_0 )
  {
      psKeywordList = CPLGetXMLNode(psMapContext, "General.KeywordList");
      if (psKeywordList)
      {
          psKeyword = psKeywordList->psChild;
          while (psKeyword)
          {
              if (psKeyword->psChild &&
                  strcasecmp(psKeyword->pszValue, "KEYWORD") == 0 )
              {
                  pszValue = psKeyword->psChild->pszValue;
                  pszHash = msLookupHashTable(&(map->web.metadata), "wms_keywordlist");
                  if (pszHash != NULL)
                  {
                      pszValue1 = (char*)malloc(strlen(pszHash)+
                                                strlen(pszValue)+2);
                      sprintf(pszValue1, "%s,%s", pszHash, pszValue);
                      msInsertHashTable(&(map->web.metadata), 
                                        "wms_keywordlist", pszValue1);
                      free(pszValue1);
                  }
                  else
                    msInsertHashTable(&(map->web.metadata), 
                                      "wms_keywordlist", pszValue);
              }
              psKeyword = psKeyword->psNext;   
          }
      }
  }
  else
    msGetMapContextXMLHashValue(psMapContext, "General.Keywords", 
                                &(map->web.metadata), "wms_keywordlist");

  // Window
  pszValue1 = (char*)CPLGetXMLValue(psMapContext,"General.Window.width",NULL);
  pszValue2 = (char*)CPLGetXMLValue(psMapContext,"General.Window.height",NULL);
  if(pszValue1 != NULL && pszValue2 != NULL)
  {
      map->width = atoi(pszValue1);
      map->height = atoi(pszValue2);
  }

  // Abstract
  if(msGetMapContextXMLHashValue(psMapContext, "General.Abstract", 
                                 &(map->web.metadata), "wms_abstract"))
  {
      msGetMapContextXMLHashValue(psMapContext, "General.gml:description", 
                                  &(map->web.metadata), "wms_abstract");
  }

  // DataURL
  msGetMapContextXMLHashValueDecode(psMapContext, 
                                   "General.DataURL.OnlineResource.xlink:href",
                                   &(map->web.metadata), "wms_dataurl");

  // LogoURL
  // The logourl have a width, height, format and an URL
  msGetMapContextXMLHashValue(psMapContext, "General.LogoURL.width", 
                              &(map->web.metadata), "wms_logourl_width");
  msGetMapContextXMLHashValue(psMapContext, "General.LogoURL.height", 
                              &(map->web.metadata), "wms_logourl_height");
  msGetMapContextXMLHashValue(psMapContext, "General.LogoURL.format", 
                              &(map->web.metadata), "wms_logourl_format");
  msGetMapContextXMLHashValue(psMapContext, 
                              "General.LogoURL.OnlineResource.xlink:href", 
                              &(map->web.metadata), "wms_logourl_href");

  // DescriptionURL
  // The descriptionurl have a width, height, format and an URL
  msGetMapContextXMLHashValue(psMapContext, "General.DescriptionURL.width", 
                             &(map->web.metadata), "wms_descriptionurl_width");
  msGetMapContextXMLHashValue(psMapContext, "General.DescriptionURL.height", 
                            &(map->web.metadata), "wms_descriptionurl_height");
  msGetMapContextXMLHashValue(psMapContext, "General.DescriptionURL.format", 
                            &(map->web.metadata), "wms_descriptionurl_format");
  msGetMapContextXMLHashValue(psMapContext, 
                            "General.DescriptionURL.OnlineResource.xlink:href",
                              &(map->web.metadata), "wms_descriptionurl_href");

  // Contact Info
  psContactInfo = CPLGetXMLNode(psMapContext, "General.ContactInformation");

  // Contact Person primary
  msGetMapContextXMLHashValue(psContactInfo, 
                              "ContactPersonPrimary.ContactPerson", 
                              &(map->web.metadata), "wms_contactperson");
  msGetMapContextXMLHashValue(psContactInfo, 
                              "ContactPersonPrimary.ContactOrganization", 
                              &(map->web.metadata), "wms_contactorganization");
  // Contact Position
  msGetMapContextXMLHashValue(psContactInfo, 
                              "ContactPosition", 
                              &(map->web.metadata), "wms_contactposition");
  // Contact Address
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.AddressType", 
                              &(map->web.metadata), "wms_addresstype");
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.Address", 
                              &(map->web.metadata), "wms_address");
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.City", 
                              &(map->web.metadata), "wms_city");
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.StateOrProvince", 
                              &(map->web.metadata), "wms_stateorprovince");
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.PostCode", 
                              &(map->web.metadata), "wms_postcode");
  msGetMapContextXMLHashValue(psContactInfo, "ContactAddress.Country", 
                              &(map->web.metadata), "wms_country");

  // Others
  msGetMapContextXMLHashValue(psContactInfo, "ContactVoiceTelephone", 
                            &(map->web.metadata), "wms_contactvoicetelephone");
  msGetMapContextXMLHashValue(psContactInfo, "ContactFacsimileTelephone", 
                        &(map->web.metadata), "wms_contactfacsimiletelephone");
  msGetMapContextXMLHashValue(psContactInfo, "ContactElectronicMailAddress", 
                     &(map->web.metadata), "wms_contactelectronicmailaddress");

  // LayerList
  psLayerList = CPLGetXMLNode(psMapContext, "LayerList");
  if( psLayerList != NULL )
  {
      for(psLayer = psLayerList->psChild; 
          psLayer != NULL; 
          psLayer = psLayer->psNext)
      {
          if(EQUAL(psLayer->pszValue, "Layer"))
          {
              // Init new layer
              layer = &(map->layers[map->numlayers]);
              initLayer(layer, map);
              layer->map = (mapObj *)map;
              layer->type = MS_LAYER_RASTER;
              /* save the index */
              map->layers[map->numlayers].index = map->numlayers;
              map->layerorder[map->numlayers] = map->numlayers;
              map->numlayers++;
              //if(&(layer->metadata) == NULL)
              //    &(layer->metadata) =  msCreateHashTable();

              // Status
              pszValue = (char*)CPLGetXMLValue(psLayer, "hidden", "1");
              if((pszValue != NULL) && (atoi(pszValue) == 0))
                  layer->status = MS_ON;
              else
                  layer->status = MS_OFF;

              // Queryable
              pszValue = (char*)CPLGetXMLValue(psLayer, "queryable", "0");
              if(pszValue !=NULL && atoi(pszValue) == 1)
                  layer->template = strdup("ttt");

              // Name and Title
              pszValue = (char*)CPLGetXMLValue(psLayer, "Name", NULL);
              if(pszValue != NULL)
              {
                  msInsertHashTable( &(layer->metadata), "wms_name", pszValue );

                  pszName = (char*)malloc(sizeof(char)*(strlen(pszValue)+10));
                  sprintf(pszName, "l%d:%s", layer->index, pszValue);
                  layer->name = strdup(pszName);
                  free(pszName);
              }
              else
              {
                  pszName = (char*)malloc(sizeof(char)*10);
                  sprintf(pszName, "l%d:", layer->index);
                  layer->name = strdup(pszName);
                  free(pszName);
              }

              if(msGetMapContextXMLHashValue(psLayer, "Title", 
                                &(layer->metadata), "wms_title") == MS_FAILURE)
              {
                  if(msGetMapContextXMLHashValue(psLayer, "Server.title", 
                                &(layer->metadata), "wms_title") == MS_FAILURE)
                  {
                      msDebug("Mandatory data Layer.Title missing in %s.",
                              filename);
                  }
              }

              // Abstract
              msGetMapContextXMLHashValue(psLayer, "Abstract", 
                                          &(layer->metadata), "wms_abstract");

              // DataURL
              if(nVersion <= OWS_0_1_4)
              {
                  msGetMapContextXMLHashValueDecode(psLayer, 
                                                    "DataURL.OnlineResource.xlink:href",
                                                    &(layer->metadata), 
                                                    "wms_dataurl");
              }
              else
              {
                  // The DataURL have a width, height, format and an URL
                  // Width and height are not used, but they are included to
                  // be consistent with the spec.
                  msGetMapContextXMLHashValue(psLayer, "DataURL.width", 
                                              &(layer->metadata), 
                                              "wms_dataurl_width");
                  msGetMapContextXMLHashValue(psLayer, "DataURL.height", 
                                              &(layer->metadata), 
                                              "wms_dataurl_height");
                  msGetMapContextXMLHashValue(psLayer, "DataURL.format", 
                                              &(layer->metadata), 
                                              "wms_dataurl_format");
                  msGetMapContextXMLHashValue(psLayer, 
                                           "DataURL.OnlineResource.xlink:href",
                                              &(layer->metadata), 
                                              "wms_dataurl_href");
              }

              // The MetadataURL have a width, height, format and an URL
              // Width and height are not used, but they are included to
              // be consistent with the spec.
              msGetMapContextXMLHashValue(psLayer, "MetadataURL.width", 
                               &(layer->metadata), "wms_metadataurl_width");
              msGetMapContextXMLHashValue(psLayer, "MetadataURL.height", 
                               &(layer->metadata), "wms_metadataurl_height");
              msGetMapContextXMLHashValue(psLayer, "MetadataURL.format", 
                               &(layer->metadata), "wms_metadataurl_format");
              msGetMapContextXMLHashValue(psLayer, 
                               "MetadataURL.OnlineResource.xlink:href",
                               &(layer->metadata), "wms_metadataurl_href");


              // Server
              if(nVersion >= OWS_0_1_4)
              {
                  if(msGetMapContextXMLStringValueDecode(psLayer, 
                                           "Server.OnlineResource.xlink:href", 
                                           &(layer->connection)) == MS_FAILURE)
                  {
                      CPLDestroyXMLNode(psRoot);
                      msSetError(MS_MAPCONTEXTERR, 
              "Mandatory data Server.OnlineResource.xlink:href missing in %s.",
                                 "msLoadMapContext()", filename);
                      return MS_FAILURE;
                  }
                  else
                  {
                      msGetMapContextXMLHashValueDecode(psLayer, 
                                     "Server.OnlineResource.xlink:href", 
                                     &(layer->metadata), "wms_onlineresource");
                      layer->connectiontype = MS_WMS;
                  }
              }
              else
              {
                  if(msGetMapContextXMLStringValueDecode(psLayer, 
                                           "Server.onlineResource", 
                                           &(layer->connection)) == MS_FAILURE)
                  {
                      CPLDestroyXMLNode(psRoot);
                      msSetError(MS_MAPCONTEXTERR, 
                         "Mandatory data Server.onlineResource missing in %s.",
                                 "msLoadMapContext()", filename);
                      return MS_FAILURE;
                  }
                  else
                  {
                      msGetMapContextXMLHashValueDecode(psLayer, 
                                                      "Server.onlineResource", 
                                     &(layer->metadata), "wms_onlineresource");
                      layer->connectiontype = MS_WMS;
                  }
              }

              if(nVersion >= OWS_0_1_4)
              {
                  if(msGetMapContextXMLHashValue(psLayer, "Server.version", 
                       &(layer->metadata), "wms_server_version") == MS_FAILURE)
                  {
                      CPLDestroyXMLNode(psRoot);
                      msSetError(MS_MAPCONTEXTERR, 
                                "Mandatory data Server.version missing in %s.",
                                 "msLoadMapContext()", filename);
                      return MS_FAILURE;
                  }
              }
              else
              {
                  if(msGetMapContextXMLHashValue(psLayer, "Server.wmtver", 
                       &(layer->metadata), "wms_server_version") == MS_FAILURE)
                  {
                      CPLDestroyXMLNode(psRoot);
                      msSetError(MS_MAPCONTEXTERR, 
                                 "Mandatory data Server.wmtver missing in %s.",
                                 "msLoadMapContext()", filename);
                      return MS_FAILURE;
                  }
              }

              // Projection
              psSRS = psLayer->psChild;
              while(psSRS != NULL)
              {
                  if(psSRS->psChild != NULL &&
                     strcasecmp(psSRS->pszValue, "SRS") == 0 )
                  {
                      pszValue = psSRS->psChild->pszValue;

                      // Add in wms_srs
                      pszHash = msLookupHashTable(&(layer->metadata), "wms_srs");
                      if(pszHash != NULL)
                      {
                          pszValue1 = (char*)malloc(strlen(pszHash)+
                                                    strlen(pszValue)+2);
                          sprintf(pszValue1, "%s %s", pszHash, pszValue);
                          msInsertHashTable(&(layer->metadata), 
                                            "wms_srs", pszValue1);
                          free(pszValue1);
                      }
                      else
                          msInsertHashTable(&(layer->metadata), 
                                            "wms_srs", pszValue);
                  }

                  psSRS = psSRS->psNext;
              }
              pszHash = msLookupHashTable(&(layer->metadata), "wms_srs");
              if((pszHash == NULL) || (strcasecmp(pszHash, "") == 0))
              {
                  if(pszMapProj != NULL)
                  {
                      msInsertHashTable(&(layer->metadata),"wms_srs", pszMapProj);
                      if(strncasecmp(pszValue, "AUTO:", 5) == 0)
                      {
                          pszProj = strdup(pszValue);
                      }
                      else
                      {
                          pszProj = (char*) malloc(sizeof(char)*
                                                   (strlen(pszValue)+10));
                          sprintf(pszProj, "init=epsg:%s", pszMapProj+5);
                      }

                      msInitProjection(&layer->projection);
                      layer->projection.args[layer->projection.numargs] = strdup(pszProj);
                      layer->projection.numargs++;
                      msProcessProjection(&layer->projection);
                      free(pszProj);
                  }
              }

              // Format
              if( nVersion >= OWS_0_1_4 )
              {
                  psFormatList = CPLGetXMLNode(psLayer, "FormatList");
              }
              else
              {
                  psFormatList = psLayer;
              }

              if(psFormatList != NULL)
              {
                  for(psFormat = psFormatList->psChild; 
                      psFormat != NULL; 
                      psFormat = psFormat->psNext)
                  {
                      if(psFormat->psChild != NULL && 
                         strcasecmp(psFormat->pszValue, "Format") == 0 )
                      {
                          if(psFormat->psChild->psNext == NULL)
                              pszValue = psFormat->psChild->pszValue;
                          else
                              pszValue = psFormat->psChild->psNext->pszValue;
                      }
                      else
                          pszValue = NULL;

                      if(pszValue != NULL && strcasecmp(pszValue, "") != 0)
                      {
                          // wms_format
                          pszValue1 = (char*)CPLGetXMLValue(psFormat, 
                                                            "current", NULL);
                          if(pszValue1 != NULL && 
                             (strcasecmp(pszValue1, "1") == 0))
                              msInsertHashTable(&(layer->metadata), 
                                                "wms_format", pszValue);
                          // wms_formatlist
                          pszHash = msLookupHashTable(&(layer->metadata), 
                                                      "wms_formatlist");
                          if(pszHash != NULL)
                          {
                              pszValue1 = (char*)malloc(strlen(pszHash)+
                                                        strlen(pszValue)+2);
                              sprintf(pszValue1, "%s,%s", pszHash, pszValue);
                              msInsertHashTable(&(layer->metadata), 
                                                "wms_formatlist", pszValue1);
                              free(pszValue1);
                          }
                          else
                              msInsertHashTable(&(layer->metadata), 
                                                "wms_formatlist", pszValue);
                      }
                  }

                  /* Make sure selected format is supported or select another
                   * supported format.  Note that we can efficiently do this
                   * only for GIF/PNG/JPEG, can't try to handle all GDAL
                   * formats.
                   */
                  pszValue = msLookupHashTable(&(layer->metadata), "wms_format");

                  if (
#ifndef USE_GD_PNG
                      strcasecmp(pszValue, "image/png") == 0 || 
                      strcasecmp(pszValue, "PNG") == 0 ||
#endif
#ifndef USE_GD_JPEG
                      strcasecmp(pszValue, "image/jpeg") == 0 || 
                      strcasecmp(pszValue, "JPEG") == 0 ||
#endif
#ifndef USE_GD_GIF
                      strcasecmp(pszValue, "image/gif") == 0 || 
                      strcasecmp(pszValue, "GIF") == 0 ||
#endif
                      0 )
                  {
                      char **papszList=NULL;
                      int i, numformats=0;

                      pszValue = msLookupHashTable(&(layer->metadata), 
                                                   "wms_formatlist");

                      papszList = split(pszValue, ',', &numformats);
                      for(i=0; i < numformats; i++)
                      {
                          if (
#ifdef USE_GD_PNG
                              strcasecmp(papszList[i], "image/png") == 0 || 
                              strcasecmp(papszList[i], "PNG") == 0 ||
#endif
#ifdef USE_GD_JPEG
                              strcasecmp(papszList[i], "image/jpeg") == 0 || 
                              strcasecmp(papszList[i], "JPEG") == 0 ||
#endif
#ifdef USE_GD_GIF
                              strcasecmp(papszList[i], "image/gif") == 0 || 
                              strcasecmp(papszList[i], "GIF") == 0 ||
#endif
                              0 )
                          {
                              /* Found a match */
                              msInsertHashTable(&(layer->metadata), 
                                                "wms_format", papszList[i]);
                              break;
                          }
                      }
                      if(papszList)
                          msFreeCharArray(papszList, numformats);

                  } /* end if unsupported format */
                   
              } /* end FormatList parsing */

              // Style
              if( nVersion >= OWS_0_1_4 )
              {
                  psStyleList = CPLGetXMLNode(psLayer, "StyleList");
              }
              else
              {
                  psStyleList = psLayer;
              }

              if(psStyleList != NULL)
              {
                  nStyle = 0;
                  for(psStyle = psStyleList->psChild; 
                      psStyle != NULL; 
                      psStyle = psStyle->psNext)
                  {
                    if(strcasecmp(psStyle->pszValue, "Style") == 0)
                    {
                      pszStyleName =(char*)CPLGetXMLValue(psStyle,"Name",NULL);
                      if(pszStyleName == NULL)
                      {
                          nStyle++;
                          pszStyleName = (char*)malloc(15);
                          sprintf(pszStyleName, "Style{%d}", nStyle);
                      }
                      else
                          pszStyleName = strdup(pszStyleName);

                      // wms_style
                      pszValue = (char*)CPLGetXMLValue(psStyle,"current",NULL);
                      if(pszValue != NULL && 
                         (strcasecmp(pszValue, "1") == 0))
                          msInsertHashTable(&(layer->metadata), 
                                            "wms_style", pszStyleName);
                      // wms_stylelist
                      pszHash = msLookupHashTable(&(layer->metadata), 
                                                  "wms_stylelist");
                      if(pszHash != NULL)
                      {
                          pszValue1 = (char*)malloc(strlen(pszHash)+
                                                    strlen(pszStyleName)+2);
                          sprintf(pszValue1, "%s,%s", pszHash, pszStyleName);
                          msInsertHashTable(&(layer->metadata), 
                                            "wms_stylelist", pszValue1);
                          free(pszValue1);
                      }
                      else
                          msInsertHashTable(&(layer->metadata), 
                                            "wms_stylelist", pszStyleName);

                      // title
                      pszValue = (char*)CPLGetXMLValue(psStyle, "Title", NULL);
                      if(pszValue != NULL)
                      {
                          pszStyle = (char*)malloc(strlen(pszStyleName)+20);
                          sprintf(pszStyle,"wms_style_%s_title",pszStyleName);
                          msInsertHashTable(&(layer->metadata),pszStyle,pszValue);
                          free(pszStyle);
                      }
                      else
                      {
                          pszStyle = (char*)malloc(strlen(pszStyleName)+20);
                          sprintf(pszStyle,"wms_style_%s_title",pszStyleName);
                          msInsertHashTable(&(layer->metadata), pszStyle, 
                                            layer->name);
                          free(pszStyle);
                      }
                      // SLD
                      pszValue = (char*)CPLGetXMLValue(psStyle, 
                                        "SLD.OnlineResource.xlink:href", NULL);
                      if(pszValue != NULL)
                      {
                          pszStyle = (char*)malloc(strlen(pszStyleName)+15);
                          sprintf(pszStyle, "wms_style_%s_sld", pszStyleName);
                          msDecodeHTMLEntities(pszStyle);
                          msInsertHashTable(&(layer->metadata),pszStyle,pszValue);
                          free(pszStyle);
                      }
                      // LegendURL
                      if( nVersion >= OWS_0_1_4 )
                      {
                          pszValue=(char*)CPLGetXMLValue(psStyle,
                                   "LegendURL.OnlineResource.xlink:href",NULL);
                      }
                      else
                      {
                              psLegendURL = CPLGetXMLNode(psStyle,"LegendURL");
                              if(psLegendURL != NULL)
                              {
                                  psLegendURL = psLegendURL->psChild;
                                  while(psLegendURL != NULL && 
                                        psLegendURL->eType != CXT_Text)
                                  {
                                      psLegendURL = psLegendURL->psNext;
                                  }
                                  if(psLegendURL != NULL)
                                      pszValue = psLegendURL->pszValue;
                                  else
                                      pszValue = NULL;
                              }
                              else
                              {
                                  pszValue = NULL;
                              }
                      }
                      if(pszValue != NULL)
                      {
                          pszStyle = (char*)malloc(strlen(pszStyleName)+35);
                          sprintf(pszStyle, "wms_style_%s_legendurl_width",
                                  pszStyleName);
                          msGetMapContextXMLHashValue(psStyle, 
                                                      "LegendURL.width", 
                                                      &(layer->metadata), 
                                                      pszStyle);
                          sprintf(pszStyle, "wms_style_%s_legendurl_height",
                                  pszStyleName);
                          msGetMapContextXMLHashValue(psStyle,
                                                      "LegendURL.height", 
                                                      &(layer->metadata), 
                                                      pszStyle);
                          sprintf(pszStyle, "wms_style_%s_legendurl_format",
                                  pszStyleName);
                          msGetMapContextXMLHashValue(psStyle,
                                                      "LegendURL.format", 
                                                      &(layer->metadata), 
                                                      pszStyle);
                          sprintf(pszStyle, "wms_style_%s_legendurl_href",
                                  pszStyleName);
                          msGetMapContextXMLHashValue(psStyle,
                                         "LegendURL.OnlineResource.xlink:href",
                                                      &(layer->metadata), 
                                                      pszStyle);
                          free(pszStyle);
                      }

                      free(pszStyleName);
                    }
                  }
              }

              if(msLookupHashTable(&(layer->metadata), 
                                   "wms_stylelist") == NULL)
              {
                  if(layer->connection)
                      pszValue = strdup(layer->connection);
                  else
                      pszValue = strdup( "" ); 
                  pszValue1 = strstr(pszValue, "STYLELIST=");
                  if(pszValue1 != NULL)
                  {                          
                      pszValue1 += 10;
                      pszValue2 = strchr(pszValue, '&');
                      if(pszValue2 != NULL)
                          pszValue1[pszValue2-pszValue1] = '\0';
                      msInsertHashTable(&(layer->metadata), "wms_stylelist",
                                        pszValue1);
                  }
                  free(pszValue);
              }
              if(msLookupHashTable(&(layer->metadata), "wms_style") == NULL)
              {
                  if(layer->connection)
                      pszValue = strdup(layer->connection);
                  else
                      pszValue = strdup( "" ); 
                  pszValue1 = strstr(pszValue, "STYLE=");
                  if(pszValue1 != NULL)
                  {                          
                      pszValue1 += 6;
                      pszValue2 = strchr(pszValue, '&');
                      if(pszValue2 != NULL)
                          pszValue1[pszValue2-pszValue1] = '\0';
                      msInsertHashTable(&(layer->metadata), "wms_style",
                                        pszValue1);
                  }
                  free(pszValue);
              }

          }/* end Layer parsing */
      }/* for */
  }

  if(pszMapProj)
      free(pszMapProj);
  CPLDestroyXMLNode(psRoot);

  return MS_SUCCESS;

#else
  msSetError(MS_MAPCONTEXTERR, 
             "Not implemented since Map Context is not enabled.",
             "msGetMapContext()");
  return MS_FAILURE;
#endif
}


/* msSaveMapContext()
**
** Save a mapfile into the OGC Web Map Context format
**
** Take a map object and a file in arguments
*/
int msSaveMapContext(mapObj *map, char *filename)
{
#if defined(USE_WMS_LYR) && defined(USE_OGR)
  FILE *stream;
  char szPath[MS_MAXPATHLEN];
  int nStatus;

  // open file
  if(filename != NULL && strlen(filename) > 0) {
    stream = fopen(msBuildPath(szPath, map->mappath, filename), "wb");
    if(!stream) {
      msSetError(MS_IOERR, "(%s)", "msSaveMapContext()", filename);
      return(MS_FAILURE);
    }
  }
  else
  {
      msSetError(MS_IOERR, "Filename is undefined.", "msSaveMapContext()");
      return MS_FAILURE;
  }

  nStatus = msWriteMapContext(map, stream);

  fclose(stream);

  return nStatus;
#else
  msSetError(MS_MAPCONTEXTERR, 
             "Not implemented since Map Context is not enabled.",
             "msSaveMapContext()");
  return MS_FAILURE;
#endif
}



int msWriteMapContext(mapObj *map, FILE *stream)
{
#if defined(USE_WMS_LYR) && defined(USE_OGR)
  const char * version, *value;
  char * tabspace=NULL, *pszValue, *pszChar,*pszSLD=NULL,*pszURL,*pszSLD2=NULL;
  char *pszStyle, *pszCurrent, *pszStyleItem;
  char *pszEncodedVal;
  int i, nValue, nVersion=-1;

  // Decide which version we're going to return...
  version = msLookupHashTable(&(map->web.metadata), "wms_context_version");
  if(version == NULL)
    version = "1.0.0";

  nVersion = msOWSParseVersionString(version);
  if (nVersion < 0)
      return MS_FAILURE;  // msSetError() already called.

  // Make sure this is a supported version
  // Note that we don't write 0.1.2 even if we read it.
  switch (nVersion)
  {
    case OWS_0_1_4:
    case OWS_0_1_7:
    case OWS_1_0_0:
      // All is good, this is a supported version.
      break;
    default:
      // Not a supported version
      msSetError(MS_MAPCONTEXTERR, 
                 "This version of Map Context is not supported (%s).",
                 "msSaveMapContext()", version);
      return MS_FAILURE;
  }

  // file header
  msOWSPrintMetadata(stream, &(map->web.metadata), 
                     NULL, "wms_encoding", OWS_NOERR,
                "<?xml version='1.0' encoding=\"%s\" standalone=\"no\" ?>\n",
                    "ISO-8859-1");

  // set the WMS_Viewer_Context information
  if(nVersion >= OWS_1_0_0)
  {
      fprintf( stream, "<ViewContext version=\"%s\"", version );
  }
  else if(nVersion >= OWS_0_1_7)
  {
      fprintf( stream, "<View_Context version=\"%s\"", version );
      
  }
  else // 0.1.4
  {
      fprintf( stream, "<WMS_Viewer_Context version=\"%s\"", version );
  }

  if ( nVersion >= OWS_0_1_7 && nVersion < OWS_1_0_0 )
  {
      pszValue = msLookupHashTable(&(map->web.metadata), "wms_context_fid");
      if(pszValue != NULL)
          fprintf( stream, " fid=\"%s\"", pszValue );
      else
          fprintf( stream, " fid=\"0\"");
  }
  if ( nVersion >= OWS_1_0_0 )
        fprintf( stream, " id=\"%s\"", map->name);

  fprintf( stream, " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");

  if( nVersion >= OWS_0_1_7 && nVersion < OWS_1_0_0 )
  {
      fprintf( stream, " xmlns:gml=\"http://www.opengis.net/gml\"");
  }
  if( nVersion >= OWS_1_0_0 )
  {
      fprintf( stream, " xmlns:xlink=\"http://www.w3.org/1999/xlink\"");
      fprintf( stream, " xmlns=\"http://www.opengis.net/context\"");
      //fprintf( stream, " xmlns:sld=\"http://www.opengis.net/sld");
      fprintf( stream, 
               " xsi:schemaLocation=\"http://www.opengis.net/context http://schemas.opengis.net/context/1.0.0/context.xsd\">\n");
  }
  else
  {
      fprintf( stream, " xmlns:xlink=\"http://www.w3.org/TR/xlink\"");
                   
      fprintf( stream, 
               " xsi:noNamespaceSchemaLocation=\"%s/contexts/%s/context.xsd\">\n",
               msOWSGetSchemasLocation(map), version );
  }

  // set the General information
  fprintf( stream, "  <General>\n" );

  // Window
  if( map->width != -1 || map->height != -1 )
      fprintf( stream, "    <Window width=\"%d\" height=\"%d\"/>\n", 
               map->width, map->height );

  // Bounding box corners and spatial reference system
  if(tabspace)
      free(tabspace);
  tabspace = strdup("    ");
  value = msGetEPSGProj(&(map->projection), &(map->web.metadata), MS_TRUE);
  fprintf( stream, 
          "%s<!-- Bounding box corners and spatial reference system -->\n", 
           tabspace );
  if(!value || (strcasecmp(value, "(null)") == 0))
              fprintf(stream, "<!-- WARNING: Mandatory data 'projection' was missing in this context. -->\n");

  fprintf( stream, "%s<BoundingBox SRS=\"%s\" minx=\"%f\" miny=\"%f\" maxx=\"%f\" maxy=\"%f\"/>\n", 
           tabspace, value, map->extent.minx, map->extent.miny, 
           map->extent.maxx, map->extent.maxy );

  // Title, name
  if( nVersion >= OWS_0_1_7 && nVersion < OWS_1_0_0 )
  {
      fprintf( stream, "%s<gml:name>%s</gml:name>\n", tabspace, map->name );
  }
  else 
  {
      if (nVersion < OWS_0_1_7)
        fprintf( stream, "%s<Name>%s</Name>\n", tabspace, map->name );

      fprintf( stream, "%s<!-- Title of Context -->\n", tabspace );
      msOWSPrintMetadata(stream, &(map->web.metadata), 
                         NULL, "wms_title", OWS_WARN,
                         "    <Title>%s</Title>\n", map->name);
  }

  //keyword
  if (nVersion >= OWS_1_0_0)
  {
      if (msLookupHashTable(&(map->web.metadata),"wms_keywordlist")!=NULL)
      {
          char **papszKeywords;
          int nKeywords, iKey;

          pszValue = msLookupHashTable(&(map->web.metadata), 
                                       "wms_keywordlist");
          papszKeywords = split(pszValue, ',', &nKeywords);
          if(nKeywords > 0 && papszKeywords)
          {
              fprintf( stream, "    <KeywordList>\n");
              for(iKey=0; iKey<nKeywords; iKey++)
              { 
                  fprintf( stream, "      <Keyword>%s</Keyword>\n", 
                          papszKeywords[iKey]);
              }
              fprintf( stream, "    </KeywordList>\n");
          }
      }
  }
  else
    msOWSPrintMetadataList(stream, &(map->web.metadata), NULL, "wms_keywordlist", 
                           "    <Keywords>\n", "    </Keywords>\n",
                           "      %s\n", NULL);

  //abstract
  if( nVersion >= OWS_0_1_7 && nVersion < OWS_1_0_0 )
  {
      msOWSPrintMetadata(stream, &(map->web.metadata), 
                         NULL, "wms_abstract", OWS_NOERR,
                         "    <gml:description>%s</gml:description>\n", NULL);
  }
  else
  {
      msOWSPrintMetadata(stream, &(map->web.metadata), 
                         NULL, "wms_abstract", OWS_NOERR,
                         "    <Abstract>%s</Abstract>\n", NULL);
  }

  // DataURL
  msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                           NULL, "wms_dataurl", OWS_NOERR,
                "    <DataURL>\n      <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n    </DataURL>\n", NULL);

  // LogoURL
  // The LogoURL have a width, height, format and an URL
  pszURL = msLookupHashTable(&(map->web.metadata), "wms_logourl_href");
  if(pszURL != NULL)
  {
      fprintf( stream, "    <LogoURL");
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_logourl_width", OWS_NOERR,
                               " width=\"%s\"", NULL);
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_logourl_height", OWS_NOERR,
                               " height=\"%s\"", NULL);
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_logourl_format", OWS_NOERR,
                               " format=\"%s\"", NULL);
      fprintf( stream, ">\n");
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_logourl_href", OWS_NOERR,
          "      <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                               NULL);
      fprintf( stream, "    </LogoURL>\n");
  }

  // DescriptionURL
  // The DescriptionURL have a width, height, format and an URL
  // The metadata is structured like this: "width height format url"
  pszURL = msLookupHashTable(&(map->web.metadata), "wms_descriptionurl_href");
  if(pszURL != NULL &&  nVersion >= OWS_1_0_0)
  {
      fprintf( stream, "    <DescriptionURL");
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata),
                               NULL, "wms_descriptionurl_width", OWS_NOERR, 
                               " width=\"%s\"", NULL);
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_descriptionurl_height", OWS_NOERR, 
                               " height=\"%s\"", NULL);
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_descriptionurl_format", OWS_NOERR,
                               " format=\"%s\"", NULL);
      fprintf( stream, ">\n");
      msOWSPrintEncodeMetadata(stream, &(map->web.metadata), 
                               NULL, "wms_descriptionurl_href", OWS_NOERR,
          "      <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                               NULL);
      fprintf( stream, "    </DescriptionURL>\n");
  }

  // Contact Info
  msOWSPrintContactInfo( stream, tabspace, OWS_1_1_0, &(map->web.metadata) );

  // Close General
  fprintf( stream, "  </General>\n" );
  free(tabspace);

  // Set the layer list
  fprintf(stream, "  <LayerList>\n");

  // Loop on all layer  
  for(i=0; i<map->numlayers; i++)
  {
      if(map->layers[i].connectiontype == MS_WMS)
      {
          if(map->layers[i].status == MS_OFF)
              nValue = 1;
          else
              nValue = 0;
          fprintf(stream, "    <Layer queryable=\"%d\" hidden=\"%d\">\n", 
                  msIsLayerQueryable(&(map->layers[i])), nValue);

          // 
          // Server definition
          //
          msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                             NULL, "wms_server_version", OWS_WARN,
                             "      <Server service=\"WMS\" version=\"%s\" ",
                             "1.1.0");
          if(map->layers[i].name)
              msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                                 NULL, "wms_title", OWS_NOERR, 
                                 "title=\"%s\">\n", map->layers[i].name);
          else
          {
              msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                                 NULL, "wms_title", OWS_NOERR, 
                                 "title=\"%s\">\n", "");
          }

          // Get base url of the online resource to be the default value
          if(map->layers[i].connection)
              pszValue = strdup( map->layers[i].connection );
          else
              pszValue = strdup( "" );
          pszChar = strchr(pszValue, '?');
          if( pszChar )
              pszValue[pszChar - pszValue] = '\0';
          if(msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                      NULL, "wms_onlineresource", OWS_WARN, 
         "        <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                                      pszValue) == OWS_WARN)
              fprintf(stream, "<!-- wms_onlineresource not set, using base URL , but probably not what you want -->\n");
          fprintf(stream, "      </Server>\n");
          if(pszValue)
              free(pszValue);

          //
          // Layer information
          //
          msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                             NULL, "wms_name", OWS_WARN, 
                             "      <Name>%s</Name>\n", 
                             map->layers[i].name);
          msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                             NULL, "wms_title", OWS_WARN, 
                             "      <Title>%s</Title>\n", 
                             map->layers[i].name);
          msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                             NULL, "wms_abstract", OWS_NOERR, 
                             "      <Abstract>%s</Abstract>\n", 
                             NULL);

          // Layer SRS
          pszValue = (char*)msGetEPSGProj(&(map->layers[i].projection), 
                                          &(map->layers[i].metadata), MS_FALSE);
          if(pszValue && (strcasecmp(pszValue, "(null)") != 0))
              fprintf(stream, "      <SRS>%s</SRS>\n", pszValue);

          // DataURL
          if(nVersion <= OWS_0_1_4)
          {
              msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                                 NULL, "wms_dataurl", OWS_NOERR, 
                                 "      <DataURL>%s</DataURL>\n", 
                                 NULL);
          }
          else
          {
              // The DataURL have a width, height, format and an URL
              // The metadata will be structured like this: 
              // "width height format url"
              // Note: in version 0.1.7 the width and height are not included 
              // in the Context file, but they are included in the metadata for
              // for consistency with the URLType.
              pszURL = msLookupHashTable( &(map->layers[i].metadata),
                                          "wms_dataurl_href");
              if(pszURL != NULL)
              {
                  fprintf( stream, "      <DataURL");
                  msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                           NULL, "wms_dataurl_width", OWS_NOERR,
                                           " width=\"%s\"", NULL);
                  msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                           NULL, "wms_dataurl_height", OWS_NOERR,
                                           " height=\"%s\"", NULL);
                  msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                           NULL, "wms_dataurl_format", OWS_NOERR,
                                           " format=\"%s\"", NULL);
                  fprintf( stream, ">\n");
                  msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                           NULL, "wms_dataurl_href", OWS_NOERR,
                                           "        <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                                           NULL);
                  fprintf( stream, "      </DataURL>\n");
              }
          }

          // MetadataURL
          // The MetadataURL have a width, height, format and an URL
          // The metadata will be structured like this: 
          // "width height format url"
          pszURL = msLookupHashTable(&(map->layers[i].metadata),
                                     "wms_metadataurl_href");
          if(pszURL != NULL)
          {
              fprintf( stream, "      <MetadataURL");
              msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                       NULL, "wms_metadataurl_width", OWS_NOERR,
                                       " width=\"%s\"", NULL);
              msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                       NULL, "wms_metadataurl_height", OWS_NOERR,
                                       " height=\"%s\"", NULL);
              msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                       NULL, "wms_metadataurl_format", OWS_NOERR,
                                       " format=\"%s\"", NULL);
              fprintf( stream, ">\n");
              msOWSPrintEncodeMetadata(stream, &(map->layers[i].metadata), 
                                       NULL, "wms_metadataurl_href", OWS_NOERR,
                                       "        <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                                       NULL);
              fprintf( stream, "      </MetadataURL>\n");
          }

          // Format
          if(msLookupHashTable(&(map->layers[i].metadata),"wms_formatlist")==NULL)
          {
              pszURL = NULL;
              if(map->layers[i].connection)
                  pszURL = strdup( map->layers[i].connection );
              else
                  pszURL = strdup( "" );
              pszValue = pszURL;
              pszValue = strstr( pszValue, "FORMAT=" );
              if( pszValue )
              {
                  pszValue += 7;
                  pszChar = strchr(pszValue, '&');
                  if( pszChar )
                      pszValue[pszChar - pszValue] = '\0';
                  if(strcasecmp(pszValue, "") != 0)
                  {
                      fprintf( stream, "      <FormatList>\n");
                      fprintf(stream,"        <Format>%s</Format>\n",pszValue);
                      fprintf( stream, "      </FormatList>\n");
                  }
              }
              if(pszURL)
                  free(pszURL);
          }
          else
          {
              char **papszFormats;
              int numFormats, nForm;

              pszValue = msLookupHashTable(&(map->layers[i].metadata), 
                                           "wms_formatlist");
              pszCurrent = msLookupHashTable(&(map->layers[i].metadata), 
                                             "wms_format");

              papszFormats = split(pszValue, ',', &numFormats);
              if(numFormats > 0 && papszFormats)
              {
                  fprintf( stream, "      <FormatList>\n");
                  for(nForm=0; nForm<numFormats; nForm++)
                  {
                      if(pszCurrent && (strcasecmp(papszFormats[nForm], 
                                                   pszCurrent) == 0))
                          fprintf( stream,
                                 "        <Format current=\"1\">%s</Format>\n",
                                   papszFormats[nForm]);
                      else
                          fprintf( stream, "        <Format>%s</Format>\n", 
                                   papszFormats[nForm]);
                  }
                  fprintf( stream, "      </FormatList>\n");
              }
              if(papszFormats)
                  msFreeCharArray(papszFormats, numFormats);
          }

          // Style
          if(msLookupHashTable(&(map->layers[i].metadata),"wms_stylelist") ==NULL)
          {
              pszURL = "";
              if(map->layers[i].connection)
                  pszURL = strdup( map->layers[i].connection );
              else
                  pszURL = strdup( "" );
              pszValue = pszURL;
              pszValue = strstr( pszValue, "STYLES=" );
              if( pszValue )
              {
                  pszValue += 7;
                  pszChar = strchr(pszValue, '&');
                  if( pszChar )
                      pszValue[pszChar - pszValue] = '\0';

                  if(map->layers[i].connection)
                      pszSLD2 = strdup(map->layers[i].connection);
                  else
                      pszSLD2 = strdup( "" );
                  if(pszSLD2)
                      pszSLD = strstr(pszSLD2, "SLD=");
                  else
                      pszSLD = NULL;
                  if( pszSLD )
                  {
                      pszChar = strchr(pszSLD, '&');
                      if( pszChar )
                          pszSLD[pszChar - pszSLD] = '\0';
                      pszSLD += 4;
                  }
                  if( (pszValue && (strcasecmp(pszValue, "") != 0)) || 
                      (pszSLD && (strcasecmp(pszSLD, "") != 0)))
                  {
                      fprintf( stream, "      <StyleList>\n");
                      fprintf( stream, "        <Style current=\"1\">\n");
                      if( pszValue && (strcasecmp(pszValue, "") != 0))
                      {
                          fprintf(stream, "          <Name>%s</Name>\n", 
                                  pszValue);
                          fprintf(stream,"          <Title>%s</Title>\n",
                                  pszValue);
                      }
                      if( pszSLD && (strcasecmp(pszSLD, "") != 0))
                      {
                          pszEncodedVal = msEncodeHTMLEntities(pszSLD);
                          fprintf( stream, "          <SLD>\n" );
                          fprintf( stream, 
                         "            <OnlineResource xlink:type=\"simple\" ");
                          fprintf(stream,"xlink:href=\"%s\"/>", pszEncodedVal);
                          fprintf( stream, "          </SLD>\n" );
                          free(pszEncodedVal);
                      }
                      fprintf( stream, "        </Style>\n");
                      fprintf( stream, "      </StyleList>\n");
                  }
                  if(pszSLD2)
                  {
                      free(pszSLD2);
                      pszSLD2 = NULL;
                  }
              }
              if(pszURL)
              {
                  free(pszURL);
                  pszURL = NULL;
              }
          }
          else
          {
              pszValue = msLookupHashTable(&(map->layers[i].metadata), 
                                           "wms_stylelist");
              pszCurrent = msLookupHashTable(&(map->layers[i].metadata), 
                                             "wms_style");
              fprintf( stream, "      <StyleList>\n");
              while(pszValue != NULL)
              {
                  pszStyle = strdup(pszValue);
                  pszChar = strchr(pszStyle, ',');
                  if(pszChar != NULL)
                      pszStyle[pszChar - pszStyle] = '\0';
                  if( strcasecmp(pszStyle, "") != 0)
                  {
                      if(pszCurrent && (strcasecmp(pszStyle, pszCurrent) == 0))
                          fprintf( stream,"        <Style current=\"1\">\n" );
                      else
                          fprintf( stream, "        <Style>\n" );

                      pszStyleItem = (char*)malloc(strlen(pszStyle)+10+5);
                      sprintf(pszStyleItem, "wms_style_%s_sld", pszStyle);
                      if(msLookupHashTable(&(map->layers[i].metadata),
                                           pszStyleItem) != NULL)
                      {
                          fprintf(stream, "          <SLD>\n");
                          msOWSPrintEncodeMetadata(stream, 
                                                   &(map->layers[i].metadata),
                                                   NULL, pszStyleItem, 
                                                   OWS_NOERR, 
     "            <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                                                   NULL);
                          fprintf(stream, "          </SLD>\n");
                          free(pszStyleItem);
                      }
                      else
                      {
                          free(pszStyleItem);
                          // Name
                          fprintf(stream, "          <Name>%s</Name>\n", 
                                  pszStyle);
                          pszStyleItem = (char*)malloc(strlen(pszStyle)+10+8);
                          sprintf(pszStyleItem, "wms_style_%s_title",pszStyle);
                          // Title
                          msOWSPrintMetadata(stream, &(map->layers[i].metadata), 
                                             NULL, pszStyleItem, OWS_NOERR, 
                                             "          <Title>%s</Title>\n", 
                                             NULL);
                          free(pszStyleItem);

                          // LegendURL
                          pszStyleItem = (char*)malloc(strlen(pszStyle)+10+20);
                          sprintf(pszStyleItem, "wms_style_%s_legendurl_href",
                                  pszStyle);
                          pszURL = msLookupHashTable(&(map->layers[i].metadata), 
                                                     pszStyleItem);
                          if(pszURL != NULL)
                          {
                              fprintf( stream, "          <LegendURL");
                              sprintf(pszStyleItem, 
                                      "wms_style_%s_legendurl_width",pszStyle);
                              msOWSPrintEncodeMetadata(stream, 
                                                       &(map->layers[i].metadata),
                                                       NULL, pszStyleItem,
                                                       OWS_NOERR,
                                                       " width=\"%s\"", NULL);
                              sprintf(pszStyleItem, 
                                     "wms_style_%s_legendurl_height",pszStyle);
                              msOWSPrintEncodeMetadata(stream, 
                                                       &(map->layers[i].metadata),
                                                       NULL, pszStyleItem,
                                                       OWS_NOERR,
                                                       " height=\"%s\"", NULL);
                              sprintf(pszStyleItem, 
                                     "wms_style_%s_legendurl_format",pszStyle);
                              msOWSPrintEncodeMetadata(stream, 
                                                       &(map->layers[i].metadata),
                                                       NULL, pszStyleItem,
                                                       OWS_NOERR,
                                                       " format=\"%s\"", NULL);
                              fprintf( stream, ">\n");
                              sprintf(pszStyleItem, 
                                     "wms_style_%s_legendurl_href", pszStyle);
                              msOWSPrintEncodeMetadata(stream, 
                                                       &(map->layers[i].metadata),
                                                       NULL, pszStyleItem,
                                                       OWS_NOERR,
                                                       "            <OnlineResource xlink:type=\"simple\" xlink:href=\"%s\"/>\n",
                                                       NULL);
                              fprintf( stream, "          </LegendURL>\n");
                          }
                          free(pszStyleItem);
                      }

                      fprintf( stream,"        </Style>\n" );
                  }
                  free(pszStyle);
                  pszValue = strchr(pszValue, ',');
                  if(pszValue)  
                      pszValue++;
              }
              fprintf( stream, "      </StyleList>\n");
          }

          fprintf(stream, "    </Layer>\n");
      }
  }

  // Close layer list
  fprintf(stream, "  </LayerList>\n");
  // Close Map Context

  if(nVersion >= OWS_1_0_0)
  {
      fprintf(stream, "</ViewContext>\n");
  }
  else if(nVersion >= OWS_0_1_7)
  {
      fprintf(stream, "</View_Context>\n");
  }
  else // 0.1.4
  {
      fprintf(stream, "</WMS_Viewer_Context>\n");
  }

  return MS_SUCCESS;
#else
  msSetError(MS_MAPCONTEXTERR, 
             "Not implemented since Map Context is not enabled.",
             "msWriteMapContext()");
  return MS_FAILURE;
#endif
}

