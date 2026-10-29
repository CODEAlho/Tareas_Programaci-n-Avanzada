import heapq
import json

class NodoHuffman:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class DescompresorHuffman:
    def __init__(self):
        self.heap = []

    # Reconstruir el árbol desde el header
    def _construir_arbol(self, tabla_frec):
        for caracter, frecuencia in tabla_frec.items():
            nodo_nuevo = NodoHuffman(caracter, frecuencia)
            heapq.heappush(self.heap, nodo_nuevo)

        while len(self.heap) > 1:
            n_izq = heapq.heappop(self.heap)
            n_der = heapq.heappop(self.heap)
            nodo_fusionado = NodoHuffman(None, n_izq.freq + n_der.freq)
            nodo_fusionado.left = n_izq
            nodo_fusionado.right = n_der
            heapq.heappush(self.heap, nodo_fusionado)

        return self.heap[0] if self.heap else None

    def _remover_padding(self, texto_en_bits_relleno):
        info_relleno = texto_en_bits_relleno[:8]
        bits_sobrantes = int(info_relleno, 2)
        cadena_codificada = texto_en_bits_relleno[8:]
        cadena_codificada = cadena_codificada[:-bits_sobrantes if bits_sobrantes > 0 else None]
        return cadena_codificada

    def descomprimir(self, ruta_comprimida, ruta_destino):
        with open(ruta_comprimida, "rb") as archivo_in:
            # LEER HEADER
            tamano_cabecera_bytes = archivo_in.read(4)
            tamano_cabecera = int.from_bytes(tamano_cabecera_bytes, byteorder="big")
            
            cabecera_bytes = archivo_in.read(tamano_cabecera)
            cabecera_json = cabecera_bytes.decode("utf-8")
            tabla_frec = json.loads(cabecera_json)
            
            # RECONSTRUIR ÁRBOL
            raiz_arbol = self._construir_arbol(tabla_frec)
            
            # LEER DATOS COMPRIMIDOS
            lista_bits = []
            fragmento = archivo_in.read(1)
            while fragmento:
                valor_byte = ord(fragmento)
                lista_bits.append("{0:08b}".format(valor_byte))
                fragmento = archivo_in.read(1)

        cadena_bits_recuperada = "".join(lista_bits)
        cadena_codificada = self._remover_padding(cadena_bits_recuperada)

        bit_actual_str = ""
        lista_caracteres = []
        puntero_nodo = raiz_arbol

        for bit in cadena_codificada:
            bit_actual_str += bit
            if bit == "0":
                puntero_nodo = puntero_nodo.left
            else:
                puntero_nodo = puntero_nodo.right

            if puntero_nodo.char is not None:
                lista_caracteres.append(puntero_nodo.char)
                puntero_nodo = raiz_arbol

        texto_final = "".join(lista_caracteres)
        with open(ruta_destino, "w", encoding="utf-8") as archivo_out:
            archivo_out.write(texto_final)

        print(f"\n¡Éxito! Archivo descomprimido guardado como: {ruta_destino}")

if __name__ == "__main__":
    print("--- PROGRAMA DESCOMPRESOR DE HUFFMAN ---")
    archivo_entrada = input("Ingresa el archivo comprimido a abrir (ej. T6comprimido.mizip): ")
    archivo_salida = input("Ingresa el nombre para el archivo recuperado (ej. T6recuperado.txt): ")
    
    descompresor = DescompresorHuffman()
    descompresor.descomprimir(archivo_entrada, archivo_salida)
