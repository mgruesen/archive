; Michael Gruesen
; LLVM String Compare

; Input strings
@string_a = internal constant [8 x i8] c"stringa\00"
@string_b = internal constant [8 x i8] c"stringb\00"
@string_c = internal constant [8 x i8] c"stringa\00"

; Reporting strings
@a_cmp_b = internal constant [24 x i8] c"'stringa' == 'stringb'\0A\00" 
@b_cmp_c = internal constant [24 x i8] c"'stringb' == 'stringa'\0A\00" 
@a_cmp_c = internal constant [24 x i8] c"'stringa' == 'stringa'\0A\00" 
@eq_str = internal constant [7 x i8] c"Equal\0A\00"
@neq_str = internal constant [11 x i8] c"Not equal\0A\00"

define i32 
@strlen(i8* %str) {
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


define i1 
@strcmp(i8* %a, i8* %b, i8* %msg)
{
  %1 = call i32 @puts(i8* %msg)
  ; Get a length
  %a_len = call i32 @strlen(i8* %a) 
  ; Get b length
  %b_len = call i32 @strlen(i8* %b) 
  ; Allocate offset counter
  %offset = alloca i32 
  ; Initialize offset counter
  store i32 0, i32* %offset 
  ; Compare lengths
  %2 = icmp eq i32 %a_len, %b_len 
  ; If lengths are equal goto body, else goto fail
  br i1 %2, label %strcmp.body, label %strcmp.fail

strcmp.body:
  ; Load offset counter
  %3 = load i32* %offset
  ; Get pointer to char at offset in a
  %4 = getelementptr i8* %a, i32 %3
  ; Get pointer to char at offset in b
  %5 = getelementptr i8* %b, i32 %3
  ; Load character from a
  %a_char = load i8* %4
  ; Load character from b
  %b_char = load i8* %5

  ; Compare character values
  %res = icmp eq i8 %a_char, %b_char
  ; If they are equal goto increment, else goto fail
  br i1 %res, label %strcmp.increment, label %strcmp.fail


strcmp.increment:
  ; Load offset
  %6 = load i32* %offset
  ; Increment
  %7 = add i32 %6, 1
  ; Store value
  store i32 %7, i32* %offset
  ; Check if inbounds
  %8 = icmp sle i32 %7, %a_len
  ; If in bounds goto body, else goto pass
  br i1 %8, label %strcmp.body, label %strcmp.pass

strcmp.fail:
  call i32 @puts(i8* getelementptr inbounds ([11 x i8]* @neq_str, i32 0, i32 0))
  ret i1 0

strcmp.pass:
  call i32 @puts(i8* getelementptr inbounds ([7 x i8]* @eq_str, i32 0, i32 0))
  ret i1 1
}

; Print function declaration
declare i32 @puts(i8*)

; Main
define i32 
@main()
{
  ; Get pointers to input strings.
  %a_str = getelementptr inbounds [8 x i8]* @string_a, i32 0, i32 0
  %b_str = getelementptr inbounds [8 x i8]* @string_b, i32 0, i32 0
  %c_str = getelementptr inbounds [8 x i8]* @string_c, i32 0, i32 0

  ; Compare the strings
  call i1 @strcmp(i8* %a_str, i8* %b_str, 
    i8* getelementptr inbounds ([24 x i8]* @a_cmp_b, i32 0, i32 0))
  call i1 @strcmp(i8* %b_str, i8* %c_str, 
    i8* getelementptr inbounds ([24 x i8]* @b_cmp_c, i32 0, i32 0))
  call i1 @strcmp(i8* %a_str, i8* %c_str, 
    i8* getelementptr inbounds ([24 x i8]* @a_cmp_c, i32 0, i32 0))
  
  ret i32 0
}