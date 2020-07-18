# tp-2020-1c-El-Kuelgue
# **GUIA APB**

- ssh utnso@host_name -p port
- git clone repo
- cd repo

***A PARTIR DE AHORA LOS COMANDOS SE CORREN PARADOS EN ESTA RUTA***
1. sudo ./deploy **: importa las commons. También existe el ./undeploy, que se encarga de eliminarlas en caso que fuera necesario.**
2. **Para gamecard ->** ./tall-grass-builder "mount_point" "block_size" "blocks"**: inicializa las carpetas del File System. Se corre 1 vez y sólo si no existe.**
3. ./install_and_run "module_name" "parameters"**: Le hace el make al módulo pasado por parámetro y luego lo ejecuta, con los parámetros que se le paso posteriormente. install_and_run se corre 1 vez, luego de esto el make no se debe hacer más, ejecutar ./run "module_name"**
4. ./gameboy_bulk_messages "script_file"**: ejecuta el script pasado por parámetro. El mismo se debe encontrar en el directorio "target" de Gameboy.**


![Pokemon](https://rtvc-assets-radionica3.s3.amazonaws.com/s3fs-public/styles/image_750x424/public/senalradionica/articulo-noticia/galeriaimagen/pokemonfront.jpg)
