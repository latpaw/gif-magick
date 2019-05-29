interface imOptions {
    minify: boolean;
    direction: boolean;
}

interface im {
    convert(buffer: Buffer, length: number, options: imOptions): Buffer;
}
