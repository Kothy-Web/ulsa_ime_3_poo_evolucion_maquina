# Práctica Unidad IV: La evolución de una máquina

Programación Orientada a Objetos · Ingeniería Mecatrónica · Tercer semestre

Llena cada espacio conforme avances en las fases de [PRACTICA.md](PRACTICA.md).

## Fase 1. Entender el problema

**1.1 El problema con mis propias palabras**

[El problema consiste en crear un programa que permita controlar las máquinas de una planta de producción. Cada máquina debe procesar piezas, registrar cuánto tiempo trabaja y mostrar cuántas piezas puede producir por hora. Además, el programa debe permitir organizar las máquinas en una línea de producción y generar reportes para conocer su rendimiento.]

**1.2 Reglas que siempre deben cumplirse en una máquina** (escribe al menos tres)

1. El nombre de la máquina no puede estar vacío, ya que permite identificarla dentro de la planta.
2. El tiempo de cada ciclo debe ser mayor que cero, para evitar errores al calcular la capacidad de producción.
3. Cada ciclo debe registrar exactamente una pieza procesada y sumar el tiempo correspondiente.

**1.3 ¿Quién más usa la clase `Maquina`? ¿Qué podría salir mal si otro equipo crea una máquina nueva sin conocer tus reglas?**

[La clase Maquina es utilizada por todas las clases derivadas, como Torno, Fresadora, Pulidora e InspeccionVisual, además de ControlDeLinea, que se encarga de coordinar las máquinas. Si otro equipo crea una máquina sin conocer las reglas, podría registrar incorrectamente las piezas, utilizar tiempos inválidos o no implementar correctamente sus métodos. Esto provocaría errores en los reportes y afectaría el funcionamiento de toda la línea de producción.]

## Fase 2. Diseñar la solución

**2.1 Diagrama inicial (Etapa 0)**

![Diagrama inicial](diseno_inicial.png)

**2.2 Diagrama final (Etapa 9)**

![Diagrama final](diseno_solucion.png)

**2.3 ¿Qué cambió entre los dos diagramas?**

| Elemento | Etapa 0 | Etapa 9 | Etapa que lo cambió |
| --- | --- | --- | --- |
| Visibilidad de los atributos de `Maquina` | _____ | _____ | _____ |
| `procesar()` | _____ | _____ | _____ |
| `tiempoCicloSeg()` | _____ | _____ | _____ |
| Destructor de `Maquina` | _____ | _____ | _____ |
| ¿Se pueden crear objetos `Maquina`? | _____ | _____ | _____ |
| Relación `ControlDeLinea` - `Maquina` | _____ | _____ | _____ |
| Relación `Torno` - `Motor` | _____ | _____ | _____ |
| Relación `BandaTransportadora` - `Motor` | _____ | _____ | _____ |

## Fase 3. Implementar

**3.0 Etapa 0. Una máquina sin reglas**

a) ¿Qué imprime `ejecutarTurno()` para el torno y la fresadora? ¿Por qué no imprime "torneando" ni "fresando"?

[Imprime "procesamiento generico" para ambas máquinas, porque procesar() todavía no era virtual. Aunque los objetos eran un Torno y una Fresadora, el programa utilizaba el método de la clase base Maquina, ya que la función recibía una referencia de ese tipo.]

b) ¿Por qué el torno acumula 65 s y no 135 s?

[Porque la llamada directa al torno utiliza su propio método procesar(), que suma 45 segundos, pero las dos llamadas dentro de ejecutarTurno() utilizan el método genérico de Maquina, que suma 10 segundos cada una. Por eso el total es 45 + 10 + 10 = 65 segundos.]

**3.1 Etapa 1. Invariantes y constructores explicit**

a) ¿Qué regla de la sección 1.2 rompió `torno.piezasProcesadas = -5;`?

[Rompió la regla de que una máquina no puede tener una cantidad negativa de piezas procesadas, porque no es posible producir menos de cero piezas.]

b) ¿De dónde salió la máquina "Fantasma"? ¿Qué hizo el compilador sin que se lo pidieras?

[El compilador creó automáticamente un objeto temporal de tipo Maquina utilizando el constructor que recibía un solo parámetro. Convirtió la cadena "Fantasma" en una máquina sin que yo lo indicara directamente. Esto ocurrió porque el constructor no tenía la palabra explicit.]

c) ¿Qué tuviste que cambiar en `Torno` y `Fresadora` y por qué?

[Tuve que cambiar el acceso a los atributos de la clase base, utilizando métodos como getNombre(), registrarPieza() y agregarTiempo(), porque los atributos ahora eran privados. También agregué explicit a los constructores para evitar conversiones automáticas no deseadas.]

**3.2 Etapa 2. Virtual y override**

a) La fresadora acumula 180 s, pero reporta 360 pzas/h. ¿Por qué un cálculo usó 60 s y el otro 10 s?

[Porque el método procesar() de la Fresadora utilizó su tiempo de ciclo de 60 segundos para acumular el tiempo trabajado, pero el método capacidadPorHora() utilizó el tiempo de 10 segundos de la clase base Maquina. Esto ocurrió porque en la declaración de la Fresadora faltó agregar const al método tiempoCicloSeg(), por lo que no sobrescribió correctamente al método de la clase base.]

b) ¿Tu compilador mostró alguna advertencia antes de usar `override`? ¿Qué mensaje dio después de agregar `override`?

[Antes de utilizar override, el compilador no mostró una advertencia clara sobre el problema. Después de agregarlo, mostró un error indicando que Fresadora::tiempoCicloSeg() no sobrescribía ningún método de la clase base, porque faltaba const en su declaración.]

**3.3 Etapa 3. La línea de producción**

a) ¿Por qué la línea procesó todo como "generico" si `procesar()` ya es virtual?

[Porque ControlDeLinea guardaba las máquinas en un std::vector<Maquina>, es decir, guardaba copias de los objetos. Al copiarlos como Maquina, se perdía la parte específica de cada máquina, lo que se conoce como object slicing. Por eso se ejecutaba el comportamiento genérico de la clase base.]

b) ¿Por qué el torno y la fresadora originales quedaron con 0 piezas?

[Porque la línea estaba trabajando con copias de las máquinas originales. Las piezas se registraban en esas copias y no en los objetos que se habían creado originalmente en main.]

c) La relación entre `ControlDeLinea` y `Maquina`, ¿es composición o agregación? ¿Por qué?

[Es agregación porque ControlDeLinea solamente guarda referencias mediante punteros a máquinas que existen fuera de ella. Las máquinas pueden seguir existiendo aunque la línea deje de existir, y la línea no es responsable de destruirlas.]

**3.4 Etapa 4. Destructores**

| Momento | Mensajes de destrucción que aparecieron |
| --- | --- |
| `delete temporal;` sin destructor virtual | _____ |
| `delete temporal;` con destructor virtual | _____ |
| Al terminar `main` (torno y fresadora) | _____ |

a) ¿Qué advertencia mostró el compilador? ¿Por qué decimos que ya te estaba avisando del problema?

[Mostró la advertencia deleting object of polymorphic class type 'Maquina' which has non-virtual destructor might cause undefined behavior. Esto significa que eliminar un objeto derivado mediante un puntero a la clase base sin tener un destructor virtual puede provocar un comportamiento indefinido. El compilador ya estaba avisando que la destrucción del objeto podía ser incorrecta.]

b) ¿Por qué el torno de `main` sí se destruyó completo aunque el destructor no fuera virtual? ¿A qué máquina corresponde el primer "Destruyendo Maquina" después de "Fin de main"?

[Porque el torno de main fue creado como un objeto normal y se destruyó directamente por su tipo real, no mediante un puntero a Maquina. Por eso se ejecutaron sus destructores y el del motor. El primer mensaje Destruyendo Maquina después de "Fin de main" corresponde a la Fresadora, que fue creada después del torno y se destruye primero al terminar main.]

**3.5 Etapa 5. La clase abstracta**

a) Copia los mensajes de error y explica con tus palabras qué dice cada uno.

[src/main.cpp:10:14: error: cannot declare variable 'pulidora' to be of abstract type 'Pulidora'

include/Pulidora.h:6:7: note: because the following virtual functions are pure within 'Pulidora':

include/Maquina.h:21:18: note: virtual void Maquina::procesar()

src/main.cpp:11:13: error: cannot declare variable 'generica' to be of abstract type 'Maquina'

include/Maquina.h:6:7: note: because the following virtual functions are pure within 'Maquina':

include/Maquina.h:21:18: note: virtual void Maquina::procesar()

include/Maquina.h:22:17: note: virtual int Maquina::tiempoCicloSeg() const]


El primer error indica que no se puede crear un objeto de tipo Pulidora porque todavía no implementa el método virtual puro procesar(). El segundo indica que tampoco se puede crear directamente una máquina genérica, porque Maquina es una clase abstracta y tiene métodos virtuales puros que no tienen una implementación general.
b) ¿Por qué es mejor este error de compilación que el comportamiento "generico" de antes?

[Porque ahora el compilador detecta el problema antes de ejecutar el programa y no permite crear máquinas que no cumplan con los métodos obligatorios. Antes el programa podía funcionar aparentemente, pero realizaba un procesamiento incorrecto sin avisar. Ahora es más fácil encontrar y corregir los errores.]

**3.6 Etapa 6. El contrato que el compilador no ve**

a) ¿Qué partes del contrato violó `InspeccionVisual`?

[Violó dos partes del contrato: no registraba la pieza procesada y tampoco agregaba el tiempo de trabajo. Además, su método tiempoCicloSeg() devolvía 0, cuando el contrato establecía que siempre debía ser mayor que cero.]

b) ¿Por qué el compilador no lo detectó?

[Porque el código estaba correctamente escrito desde el punto de vista de la sintaxis y los tipos. El compilador podía comprobar que los métodos estaban implementados, pero no podía saber que debían registrar exactamente una pieza ni que el tiempo debía ser mayor que cero]

c) ¿Qué pasaría si el próximo equipo tampoco lee el comentario del contrato?

[Podría crear otra máquina que no cumpla las reglas, provocando errores en el registro de piezas, en los tiempos de trabajo o en los cálculos de capacidad. Por eso es importante documentar el contrato y hacer que la clase base controle su cumplimiento.]

**3.7 Etapa 7. La base hace cumplir el contrato**

a) ¿Cómo puede una clase derivada sobrescribir un método que es privado en la base?

[Porque en C++ el acceso private y el comportamiento virtual son cosas diferentes. private impide que otras clases llamen directamente al método de la base, pero virtual permite que una clase derivada proporcione su propia implementación mediante override.]

b) ¿Qué ocurre ahora con una máquina cuyo tiempo de ciclo es 0? Compáralo con la Etapa 6.

[Ahora el programa detecta que el tiempo de ciclo es inválido y lanza una excepción std::logic_error con un mensaje que identifica a la máquina y explica el problema. En la Etapa 6, el tiempo de 0 podía provocar una división entre cero y mostrar un resultado como inf o nan, sin detener correctamente el procesamiento.]

c) ¿Por qué la línea ignoró el "atajo" del torno, pero la llamada directa no? ¿Cómo corregiste el diseño?

[Porque el torno tenía un método procesar() propio que ocultaba al método de la clase base. Cuando lo llamaba directamente desde un objeto Torno, se ejecutaba ese atajo. Sin embargo, la línea trabajaba mediante punteros a Maquina y ejecutaba el método de la base, que no era virtual. Corregí el diseño eliminando el método procesar() de las clases derivadas y dejando que la base controlara todo el proceso mediante procesar() y el método virtual ejecutarCiclo().]

**3.8 Etapa 8. Herencia mal usada**

a) Da al menos dos razones por las que `BandaTransportadora : public Motor` es un error de diseño.

[Una banda transportadora no es un motor, solamente utiliza uno para funcionar. Por eso no debería heredar de la clase Motor.

Al heredar de Motor, la banda puede utilizar directamente métodos como fijarVelocidad(), permitiendo que otras partes del programa cambien su velocidad sin control.

Además, no hereda de Maquina, por lo que no puede agregarse correctamente a ControlDeLinea, que necesita recibir objetos de tipo Maquina.]

b) La relación entre `BandaTransportadora` y `Motor`, ¿es composición o agregación? ¿Por qué?

[Es composición porque la banda transportadora tiene un motor como parte de su estructura. El motor se declara como un atributo privado dentro de BandaTransportadora, por lo que su ciclo de vida está ligado al de la banda. La banda utiliza el motor, pero no es un motor.]

**3.9 Etapa 9. La librería y la prueba final**

a) Después de agregar la `Cortadora`, ¿qué archivos mostró `git status`? ¿Cuántos archivos de la librería tuviste que modificar?

[Git mostró como modificados únicamente src/main.cpp y como archivos nuevos include/Cortadora.h y src/Cortadora.cpp. No fue necesario modificar ninguno de los archivos de la biblioteca, por lo que pude agregar una máquina nueva sin cambiar las ocho clases originales.]

b) ¿Qué pasó al escribir `-lplanta` antes de `src/main.cpp`? ¿Por qué?

[Al colocar -lplanta antes de src/main.cpp, aparecieron errores de tipo undefined reference, porque el enlazador trabaja de izquierda a derecha y todavía no había encontrado las referencias a las funciones que necesitaba resolver. Al colocar la biblioteca después de main.cpp, el programa pudo enlazarse correctamente.]

**3.10 Bitácora de dudas**

| # | Etapa | Duda | Cómo la resolví | Fuente |
| --- | --- | --- | --- | --- |
| 1 | _____ | El compilador no encontraba BandaTransportadora.h. | Revisé la ubicación del archivo y lo coloqué correctamente dentro de la carpeta include. Después compilé utilizando -Iinclude. | PRACTICA.md y apuntes de clase |
| 2 | _____ | ¿Cómo utilizar las clases después de introducir namespace planta? | Aprendí que debía escribir planta:: antes del nombre de cada clase cuando las utilizaba desde main.cpp. | PRACTICA.md y apuntes de C++ |
| 3 | _____ | ¿Por qué aparecía undefined reference al colocar la biblioteca antes de main.cpp? | Comprendí que el enlazador trabaja de izquierda a derecha, por lo que la biblioteca debe colocarse después del código que utiliza sus funciones. | PRACTICA.md y apuntes de clase |

## Fase 4. Probar y mejorar

**4.1 Tabla de pruebas**

| # | Caso | Resultado esperado | Resultado obtenido | ¿Pasa? |
| --- | --- | --- | --- | --- |
| 1 | Máquina con nombre vacío | Excepción con mensaje claro | _____ | _____ |
| 2 | Reporte de una línea vacía | "La linea no tiene maquinas" | _____ | _____ |
| 3 | `agregar(nullptr)` | Devuelve `false` | _____ | _____ |
| 4 | Reporte antes de ejecutar el turno | 0 piezas, 0 s y capacidad calculada | _____ | _____ |
| 5 | Turno de 0 ciclos | Nada cambia | _____ | _____ |
| 6 | Torno con 3 ciclos | 3 piezas, 135 s, 80 pzas/h | _____ | _____ |
| 7 | Máquina con `tiempoCicloSeg()` igual a 0 | Excepción con mensaje claro | _____ | _____ |
| 8 | Máquina con `tiempoCicloSeg()` negativo | Excepción con mensaje claro | _____ | _____ |
| 9 | La misma máquina agregada dos veces | _____ | _____ | _____ |

**4.2 Bitácora de mejoras**

| # | Falla o mejora detectada | Qué cambié | Por qué |
| --- | --- | --- | --- |
| 1 | _____ | _____ | _____ |
| 2 | _____ | _____ | _____ |

Retos opcionales que intenté: _____

## Fase 5. Publicar en GitHub

**5.1 Enlace a mi fork**

[https://github.com/Kothy-Web/ulsa_ime_3_poo_evolucion_maquina.git]

## Cierre y reflexión

**6.1 Errores que ya no pueden ocurrir**

| Error | Etapa en que apareció | Restricción que lo impide ahora | ¿Quién lo detecta: el compilador o el programa? |
| --- | --- | --- | --- |
| Contadores con valores inválidos | _____ | _____ | _____ |
| Máquina "fantasma" creada a escondidas | _____ | _____ | _____ |
| Método que no sobrescribe por un `const` | _____ | _____ | _____ |
| Máquinas copiadas y "rebanadas" | _____ | _____ | _____ |
| Destrucción incompleta | _____ | _____ | _____ |
| Máquina sin `procesar()` propio | _____ | _____ | _____ |
| Tiempo de ciclo inválido | _____ | _____ | _____ |
| Derivada que no registra sus piezas | _____ | _____ | _____ |

**6.2 ¿Cuál de los errores fue el más peligroso: el de compilación, la advertencia o el que compilaba sin problemas? ¿Por qué?**

[El más peligroso fue el error que compilaba sin problemas, porque podía hacer pensar que el programa funcionaba correctamente cuando en realidad tenía fallas en su lógica. Por ejemplo, algunas máquinas podían procesar piezas sin registrarlas correctamente o utilizar tiempos incorrectos. A diferencia de un error de compilación, estos problemas podían pasar desapercibidos y afectar los resultados.]

**6.3 ¿Qué aprendiste en esta práctica?**

[Aprendí a utilizar la programación orientada a objetos en C++, especialmente la herencia, el polimorfismo, las clases abstractas, los constructores y destructores, y la composición. También comprendí la importancia de establecer reglas para las clases, utilizar correctamente los métodos virtuales y organizar el código mediante namespaces y bibliotecas estáticas. Sobre todo, aprendí que un programa no solamente debe compilar, sino también funcionar correctamente.]

**6.4 ¿Qué cambiarías de tu proceso la próxima vez?**

[La próxima vez organizaría mejor mi trabajo desde el principio, revisando cuidadosamente las instrucciones y comprobando cada etapa antes de continuar con la siguiente. También probaría los cambios poco a poco para detectar los errores más rápido y evitar tener que corregir muchas cosas al mismo tiempo. Además, dedicaría más tiempo a comprender el código en lugar de solamente copiarlo.]
