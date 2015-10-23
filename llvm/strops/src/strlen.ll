; Michael Gruesen
; LLVM String Length

@globstr = global [13 x i8] c"input string\00"  ; Global string to be processed
@print_int = private constant [3 x i8] c"%d\0A" ; Format string for printing integers

define i32 @strlen(i8* %str) {
  %len = alloca i32       ; String length counter
  store i32 0, i32* %len  ; Initialize length counter
  br label %strlen.condit ; Goto condit

strlen.condit:
  ; Get current length (next char to check)
  %1 = load i32* %len
  ; Get a pointer to the char
  %2 = getelementptr inbounds i8* %str, i32 %1
  ; Load the char into the char register
  %char = load i8* %2
  ; Check if char is null (0)
  %3 = icmp eq i8 %char, 0
  ; If null goto end, else goto body
  br i1 %3, label %strlen.end, label %strlen.body 

strlen.body:
  %4 = load i32* %len     ; Load the current length
  %5 = add nsw i32 %4, 1  ; Increment
  store i32 %5, i32* %len ; Store result
  br label %strlen.condit ; Goto condit

strlen.end:
  %6 = load i32* %len ; Load length
  ret i32 %6          ; Return value
}

declare i32 @printf(i8*, ...) ; C printf func

; Main function decl.
define i32 
@main() {
  ; Get a pointer to global string
  %1 = getelementptr inbounds [13 x i8]* @globstr, i32 0, i32 0 
  ; Call strlen function, store return value
  %2 = call i32 @strlen(i8* %1) 
  ; Get pointer to the format string for integers
  %3 = getelementptr inbounds [3 x i8]* @print_int, i32 0, i32 0 
  ; Print the string and its length
  call i32 (i8*, ...)* @printf(i8* %1)
  call i32 (i8*, ...)* @printf(i8* %3, i32 %2)
  
  ret i32 0 ; Return
}