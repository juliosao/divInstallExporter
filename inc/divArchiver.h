#ifndef __DIV_ARCHIVER_H_INC__
#define __DIV_ARCHIVER_H_INC__

/**
 * \typedef stDivArchiverFileDef
 * \brief Estructura con los metadatos de un fichero dentro del archivador
 */
typedef struct _filedef
{
    uint8_t name[16]; /**< Nombre del fichero */
    uint32_t offset; /**< Posición dentro del fichero (menos el header) */
    uint32_t compressedLength; /**< Longitud del fichero (Comprimido) */
    uint32_t length; /**< Longitud del fichero */

} stDivArchiverFileDef;

/**
 * \typedef stDivArchiverFileDef
 * \brief Estructura con los metdatos del archivador
 */
typedef struct a
{
    stDivArchiveHeader hdr; /**< Cabecera comun a todos los ficheros de DIV */
	uint32_t numFiles; /**< Numero de ficheros archivados */
    stDivArchiverFileDef* files; /**< Array con las estructuras de los metadatos de los ficheros */
} stDivArchiver;

/**
 * \fn divArchiver_load(FILE* src, stDivArchiver* a )
 * \brief Carga los metadatos de un archivador 00X de div en memoria.
 * \param src Fichero del que leer los datos
 * \param a  Puntero a estructura dond evolcar los datos
 * \retval 0 Si todo ha ido bien
 * \retval <0 Si ha fallado algo
 * 
 * Cuando se deje de usar esta estructura, es necesario liberar los recursos en ella
 * contenidos mediante la funcion divArchiver_unload
 */
DIV_RESULT divArchiver_load(FILE* src, stDivArchiver* a );

/**
 * \fn void divArchiver_unload(stDivArchiver* a);
 * \brief Libera los recursos reservados por la estructura divArchiver que le pasen
 * \param a Estructura a liberar
 */
void divArchiver_unload(stDivArchiver* a);

/**
 * \fn divArchiver_extractChunk(FILE* src, stDivArchiver* desc, unsigned pos);
 * \brief Extrae un fichero del archivador que le pasen
 * \param src Descriptor de fichero a usar para la operación de leer los datos del archivador
 * \param desc Estructura con la descripcion de los metadatos del archivador
 * \param pos Numero de fichero a extraer
 * 
 * El fichero se extaerá en la ruta actual con el nombre que tenga asignado en la estructura.
 */
DIV_RESULT divArchiver_extractChunk(FILE* src, stDivArchiver* desc, unsigned pos);

#endif