type t;

external readAsArrayBuffer : t => Blob.t => unit = "readAsArrayBuffer" [@@bs.send];

external readAsBinaryString : t => Blob.t => unit = "readAsBinaryString" [@@bs.send];

external onload : t => (unit => unit) => unit = "onload" [@@bs.set];

external result : t => Js.Typed_array.array_buffer = "result" [@@bs.get];

external resultAsString : t => string = "result" [@@bs.get];

external create : unit => t = "FileReader" [@@bs.new];
