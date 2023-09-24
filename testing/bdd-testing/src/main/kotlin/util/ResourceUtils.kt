package util

import java.io.InputStream
import java.util.stream.Collectors

internal object Resources

fun resourceStream(name: String): InputStream {
    return Resources.javaClass.getResourceAsStream(name)!!
}

fun resourcePath(path: String): List<String> {
    return resourceStream(path).bufferedReader().lines().collect(Collectors.toList())
}