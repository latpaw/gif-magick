export interface imOptions {
  minify: boolean;
  direction: boolean;
}

export interface im {
  convert(buffer: Buffer, length: number, options: imOptions): ImResult;
}

export interface ImResult {
  data: Buffer;
  width?: number;
  height?: number;
}
