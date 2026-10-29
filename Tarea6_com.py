import heapq
import json
from collections import Counter

class NodoHuffman:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class CompresorHuffman:
    def __init__(self):
        self.heap = []
        self.codigos = {}
        self.inverso_codigos = {}

    def _calcular_frecuencias(self, contenido):
        return Counter(contenido)

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

    def _generar_codigos_rec(self, puntero_nodo, ruta_bits):
        if puntero_nodo is None:
            return
        if puntero_nodo.char is not None:
            self.codigos[puntero_nodo.char] = ruta_bits
            self.inverso_codigos[ruta_bits] = puntero_nodo.char
            return
        self._generar_codigos_rec(puntero_nodo.left, ruta_bits + "0")
        self._generar_codigos_rec(puntero_nodo.right, ruta_bits + "1")

    def _pad_encoded_text(self, texto_en_bits):
        bits_sobrantes = 8 - len(texto_en_bits) % 8
        texto_en_bits += "0" * bits_sobrantes
        info_relleno = "{0:08b}".format(bits_sobrantes)
        return info_relleno + texto_en_bits

    def _get_byte_array(self, texto_en_bits_relleno):
        if len(texto_en_bits_relleno) % 8 != 0:
            raise ValueError("El texto codificado no está alineado a bytes.")
        arreglo_bytes = bytearray()
        for i in range(0, len(texto_en_bits_relleno), 8):
            fragmento_byte = texto_en_bits_relleno[i : i + 8]
            arreglo_bytes.append(int(fragmento_byte, 2))
        return arreglo_bytes

    def comprimir(self, ruta_origen, ruta_destino):
        with open(ruta_origen, "r", encoding="utf-8") as archivo_in:
            contenido = archivo_in.read()

        tabla_frec = self._calcular_frecuencias(contenido)
        raiz_arbol = self._construir_arbol(tabla_frec)
        self._generar_codigos_rec(raiz_arbol, "")

        cadena_codificada = "".join([self.codigos[caracter] for caracter in contenido])
        cadena_con_relleno = self._pad_encoded_text(cadena_codificada)
        datos_en_bytes = self._get_byte_array(cadena_con_relleno)

        # HEADER
        cabecera_json = json.dumps(tabla_frec)
        cabecera_bytes = cabecera_json.encode("utf-8")
        tamano_cabecera = len(cabecera_bytes).to_bytes(4, byteorder="big")

        with open(ruta_destino, "wb") as archivo_out:
            archivo_out.write(tamano_cabecera)
            archivo_out.write(cabecera_bytes)
            archivo_out.write(datos_en_bytes)

        print(f"\n¡Éxito! Archivo comprimido guardado como: {ruta_destino}")

if __name__ == "__main__":
    print("--- PROGRAMA COMPRESOR DE HUFFMAN ---")
    archivo_entrada = input("Ingresa el nombre del archivo de texto a comprimir (ej. T6ejemplo.txt): ")
    archivo_salida = input("Ingresa el nombre para tu archivo comprimido (ej. T6comprimido.mizip): ")
    
    compresor = CompresorHuffman()
    compresor.comprimir(archivo_entrada, archivo_salida)
