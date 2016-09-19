; Michael Gruesen
; LLVM String Copy


@src = internal constant [12 x i8] c"some string\00"	; Global string to be copied from
@dst = internal global [12 x i8] zeroinitializer			; Empty global string to be copied to
@suc_msg = internal constant [13 x i8] c"Copy passed\0A\00" ; Error string
@err_msg = internal constant [13 x i8] c"Copy failed\0A\00" ; Error string

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

define i1 @strcpy(i8* %src, i8* %dst)
{
	%len = call i32 @strlen(i8* %src) ; Get length of string to copy
	%offset = alloca i32 ; Create an offset counter
	store i32 0, i32* %offset ; Initialize offset coutner
	br label %strcpy.condit ; Goto conditional

strcpy.condit:
	; Load the offset counter
	%1 = load i32* %offset 
	; Compare to actual length
	%2 = icmp slt i32 %1, %len
	; If less than length goto body, else goto end 
	br i1 %2, label %strcpy.body, label %strcpy.end 

strcpy.body:
	; Load the offset counter
	%3 = load i32* %offset
	; Get a pointer to the character at offset in source
	%4 = getelementptr inbounds i8* %src, i32 %3
	; Load character from pointer to source
	%5 = load i8* %4
	; Get a pointer to the character at offset in dest
	%6 = getelementptr inbounds i8* %dst, i32 %3
	; Set the character in dest
	store i8 %5, i8* %6
	; Increment offset counter
	%7 = add i32 %3, 1
	store i32 %7, i32* %offset
	; Goto condit
	br label %strcpy.condit

strcpy.end:
	; Load offset
	%8 = load i32* %offset
	; Increment
	%9 = add i32 %8, 1
	; Get a pointer to the end (past the text) of the dest string
	%10 = getelementptr inbounds i8* %dst, i32 %9
	; Null terminate the string
	store i8 0, i8* %10
	ret i1 1
}

declare i32 @printf(i8*, ...)

; Main function decl.
define i32
@main()
{
	%src = getelementptr inbounds [12 x i8]* @src, i32 0, i32 0
	%dst = getelementptr inbounds [12 x i8]* @dst, i32 0, i32 0
	%res = call i1 @strcpy(i8* %src, i8* %dst)
	br i1 %res, label %succ, label %fail

succ:
	%msg = getelementptr inbounds [13 x i8]* @suc_msg, i32 0, i32 0
	%src_str = getelementptr inbounds [12 x i8]* @src, i32 0, i32 0
	%dst_str = getelementptr inbounds [12 x i8]* @dst, i32 0, i32 0
	call i32 (i8*, ...)* @printf(i8* %msg)
	call i32 (i8*, ...)* @printf(i8* %src_str)
	call i32 (i8*, ...)* @printf(i8* %dst_str)
	ret i32 0

fail:
	call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @err_msg, i32 0, i32 0))
	ret i32 1
}